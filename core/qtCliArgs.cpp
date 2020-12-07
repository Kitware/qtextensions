// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

#include "qtCliArgs.h"

#include "qtIndexRange.h"

#ifdef Q_OS_WIN
#include <windows.h>
#endif

#include <QCoreApplication>
#include <QDebug>
#include <QFileInfo>
#include <QHash>
#include <QStringList>
#include <QTextStream>

#include <cstdlib>

QTE_IMPLEMENT_D_FUNC(qtCliArgs)

#define qOut() QTextStream(stdout, QIODevice::WriteOnly)
#define qErr() QTextStream(stderr, QIODevice::WriteOnly)

//BEGIN qtCliArgsPrivate

//-----------------------------------------------------------------------------
class qtCliArgsPrivate
{
public:
  struct OptionGroup
    {
    QString name;
    QList<qtCliOption> options;
    };

  qtCliArgsPrivate() : parsed(false), multiNamedArg(-1) {}

  bool checkRequired();
  void generateQtArgs();

  static void addQtOptions(qtCliArgs* args);

  QStringList originalArgs;
  QStringList parsedArgs;
  QString processName;

  bool parsed;
  bool parseResult;
  QString parseError;

  QStringList optionGroups;
  QStringList commonOptionGroups;
  QHash<QString, OptionGroup> options;
  QList<qtCliOption> namedArgs;
  int multiNamedArg;

  QHash<QString, qtCliOption*> optionMap;
  QHash<QString, qtCliOption*> shortOptions;
  QHash<QString, qtCliOption*> longOptions;

  QStringList qtArgs;
  QList<int*> qtArgc;
  QList<char**> qtArgv;
};

//-----------------------------------------------------------------------------
bool qtCliArgsPrivate::checkRequired()
{
  // Check that required options were specified
  foreach (auto const& og, this->options)
    {
    foreach (auto const& option, og.options)
      {
      if (option.isRequired() && !option.isSet())
        {
        this->parseError = QString("Required option '%1' was not specified.")
                             .arg(option.preferredName());
        return false;
        }
      }
    }

  // Check that required arguments were specified
  foreach (auto const& narg, this->namedArgs)
    {
    if (narg.isRequired() && !narg.isSet())
      {
      this->parseError = QString("Required argument '%1' was not specified.")
                           .arg(narg.preferredName());
      return false;
      }
    }

  // All required options/arguments are present
  return true;
}

//-----------------------------------------------------------------------------
void qtCliArgsPrivate::generateQtArgs()
{
  this->qtArgs.append(originalArgs[0]);
  foreach (auto const& option, this->options.value("qt").options)
    {
    QString name = option.preferredName();
    if (option.isSet())
      {
      if (name == "display")
        {
        qputenv("DISPLAY", option.value().toLocal8Bit());
        }
      else
        {
        this->qtArgs.append('-' + name);
        this->qtArgs.append(option.value());
        }
      }
    }
}

//-----------------------------------------------------------------------------
void qtCliArgsPrivate::addQtOptions(qtCliArgs* args)
{
  qtCliOptions qtOptions;
  // \TODO declare Qt options
#ifdef Q_WS_X11
  qtOptions.add("display <name>", "Use the X server display 'name'");
#endif
  qtOptions.add("style <style>", "Default widget style for the application");
  qtOptions.add("nograb", "Tells Qt to never grab the mouse or the keyboard");
  qtOptions.add("dograb", "Running under a debugger can cause Qt to add an"
                          " implicit --nograb; use --dograb to force Qt to"
                          " allow grabbing");
  args->addOptions(qtOptions, "Qt", false);
}

//END qtCliArgsPrivate

///////////////////////////////////////////////////////////////////////////////

//BEGIN qtCliArgs

//-----------------------------------------------------------------------------
qtCliArgs::qtCliArgs(int argc, char** argv)
  : d_ptr(new qtCliArgsPrivate)
{
  QTE_D(qtCliArgs);

#ifndef Q_OS_WIN
  // Convert argument list to QStringList
  foreach (auto const i, qtIndexRange(argc))
    {
    d->originalArgs.append(QString::fromLocal8Bit(argv[i]));
    }
#else
  // On Windows, argv is inadequate for dealing with Unicode input; we ignore
  // it and use GetCommandLineW() instead
  LPWSTR* wArgv = CommandLineToArgvW(GetCommandLineW(), &argc);
  foreach (auto const i, qtIndexRange(argc))
    {
    d->originalArgs.append(QString::fromWCharArray(wArgv[i]));
    }
  LocalFree(wArgv);
#endif

  // Better have at least argv[0]
  Q_ASSERT(d->originalArgs.count());

  // Extract first arg as process name, with path removed
  QFileInfo fi(d->originalArgs[0]);
  d->processName = fi.fileName();

  // Add Qt specific options
  qtCliArgsPrivate::addQtOptions(this);
}

//-----------------------------------------------------------------------------
qtCliArgs::~qtCliArgs()
{
  QTE_D(qtCliArgs);

  qDeleteAll(d->qtArgc);
  int k = d->qtArgv.count();
  while (k--)
    {
    char** v = d->qtArgv[k];
    int i = 0;
    while (v[i])
      {
      delete[] v[i++];
      }
    delete[] v;
    }
}

//-----------------------------------------------------------------------------
void qtCliArgs::addOptions(const qtCliOptions& options, QString group,
                           bool includeWithCommon)
{
  if (options.isEmpty())
    {
    return;
    }

  QTE_D(qtCliArgs);

  if (d->parsed)
    {
    qWarning() << "qtCliArgs: warning: calling" << __FUNCTION__
               << "after parse has been called is not supported";
    return;
    }

  // Add group to group lists, if we haven't seen it before
  const QString lgroup = group.toLower();
  if (!d->options.contains(lgroup))
    {
    d->optionGroups.append(group);
    if (includeWithCommon)
      {
      d->commonOptionGroups.append(group);
      }
    }

  // Add options to maps
  qtCliArgsPrivate::OptionGroup& og = d->options[lgroup];
  og.name = group;
  foreach (auto const& option, options.options())
    {
    og.options.append(option);
    qtCliOption* po = &og.options.last();
    foreach (auto const& sn, po->shortNames())
      {
      d->optionMap.insert(sn, po);
      d->shortOptions.insert(sn, po);
      }
    foreach (auto const& ln, po->longNames())
      {
      d->optionMap.insert(ln, po);
      d->longOptions.insert(ln, po);
      }
    }
}

//-----------------------------------------------------------------------------
void qtCliArgs::addNamedArguments(const qtCliOptions& args)
{
  if (args.isEmpty())
    {
    return;
    }

  QTE_D(qtCliArgs);

  if (d->parsed)
    {
    qWarning() << "qtCliArgs: warning: calling" << __FUNCTION__
               << "after parse has been called is not supported";
    return;
    }

  // Add named arguments to maps
  foreach (auto const& arg, args.options())
    {
    if (d->multiNamedArg < 0)
      {
      if (arg.isNamedList())
        {
        d->multiNamedArg = d->namedArgs.count();
        }
      d->namedArgs.append(arg);
      qtCliOption* pa = &d->namedArgs.last();
      foreach (auto const& name, pa->longNames())
        d->optionMap.insert(name, pa);
      }
    else
      {
      const QString multiArgName = d->namedArgs.last().preferredName();
      qWarning() << "qtCliArgs::addNamedArguments: previous named argument"
                 << multiArgName << "consumes all remaining arguments;"
                 << "named argument" << arg.preferredName()
                 << "not added";
      }
    }
}

//-----------------------------------------------------------------------------
bool qtCliArgs::parse(ParseOptions options)
{
  QTE_D(qtCliArgs);

  // Don't parse twice
  if (d->parsed)
    {
    // If asked, and we haven't already failed, check required options/arguments
    if (!options.testFlag(qtCliArgs::IgnoreRequired))
      {
      d->parseResult = d->parseResult && d->checkRequired();
      }
    return d->parseResult;
    }

  d->parsed = true;
  d->parseResult = false;

  bool considerOptions = true;
  int argc = d->originalArgs.count();
  for (int n = 1; n < argc; ++n)
    {
    const QString arg = d->originalArgs[n];

#ifdef Q_OS_MAC
    // OS X passes a process identifier argument when opening an app bundle
    // from the GUI or by using the "open" CLI command. Ignore this.
    if (arg.startsWith("-psn"))
      {
      continue;
      }
#endif

    if (considerOptions)
      {
      // Handle built-in options
      if (arg == "--")
        {
        considerOptions = false;
        continue;
        }
      else if (arg == "--help" || arg.startsWith("--help-"))
        {
        // Check for option match, so that users can add options starting with
        // "--help-"
        const int xe = arg.indexOf('=');
        const QString name = arg.mid(2, xe > 0 ? xe - 2 : -1);
        qtCliOption* option = d->longOptions.value(name);

        if (!option)
          {
          this->usage(arg.mid(7).toLower());
          exit(0);
          }
        }
      else if (arg == "-v" || arg == "--version")
        {
        qOut() << QString("Qt: %1\n%2: %3\n")
                    .arg(QString::fromLatin1(qVersion()))
                    .arg(QCoreApplication::applicationName())
                    .arg(QCoreApplication::applicationVersion());
        exit(0);
        }

      // Handle other options
      if (arg.startsWith('-'))
        {
        // Handle long options
        if (arg.startsWith("--"))
          {
          // Check for option match
          const int xe = arg.indexOf('=');
          const QString name = arg.mid(2, xe > 0 ? xe - 2 : -1);
          qtCliOption* option = d->longOptions.value(name);
          // Check for flag negation
          if (!option)
            {
            if (arg.startsWith("--no-"))
              {
              option = d->longOptions.value(arg.mid(5));
              if (option && option->isFlag())
                {
                option->setValue("0");
                continue;
                }
              }
            // Check for negation of an implicitly true option
            else
              {
              option = d->longOptions.value("no-" + name);
              if (option && option->isFlag())
                {
                option->setValue("0");
                continue;
                }
              }
            }
          // Get and set option value
          if (option)
            {
            if (option->isFlag())
              {
              option->setValue("1");
              continue;
              }
            else
              {
              if (xe > 0)
                {
                // Option value provided GNU-style ('--option=value')
                option->appendValue(arg.mid(xe + 1));
                continue;
                }
              else
                {
                // Option value provided as separate argument following option;
                // get next argument as option value
                if (++n < argc)
                  {
                  option->appendValue(d->originalArgs[n]);
                  continue;
                  }

                // No value was given following option that requires a value
                d->parseError =
                  QString("Option '%1' missing required value '%2'.")
                    .arg(arg, option->valueName());
                return false;
                }
              }
            }

          // Unknown option
          d->parseError = QString("Unknown option '%1'.").arg(arg);
          return false;
          }
        // Handle short options
        else if (arg != "-")
          {
          // Check for option match; shorter short options first
          int start = 1, length = 1;
          while (start < arg.length())
            {
            const QString sarg = arg.mid(start, length);
            qtCliOption* option = d->shortOptions.value(sarg);
            if (option)
              {
              // Get and set option value
              if (option->isFlag())
                {
                option->setValue("1");
                ++start;
                length = 1;
                continue;
                }
              else
                {
                if (start + length < arg.length())
                  {
                  // Option value concatenated to short option
                  option->appendValue(arg.mid(start + length));
                  break;
                  }
                else
                  {
                  // Option value provided as separate argument following
                  // option; get next argument as option value
                  if (++n < argc)
                    {
                    option->appendValue(d->originalArgs[n]);
                    break;
                    }
                  }

                // No value was given following option that requires a value
                d->parseError =
                  QString("Option '-%1' missing required value '%2'.")
                    .arg(arg.mid(start), option->valueName());
                return false;
                }
              }
            // No match yet...
            else if (start + length < arg.length())
              {
              // ...but try again looking for a longer match
              ++length;
              continue;
              }

            // Unknown option
            d->parseError =
              QString("Unknown option '-%1'.").arg(arg.mid(start));
            return false;
            }

          // Found something and ran out of argument; move on to next argument
          continue;
          }
        }
      }

    // Handle non-option arguments
    int pos = d->parsedArgs.count();
    d->parsedArgs.append(arg);
    if (d->multiNamedArg >= 0 && pos >= d->multiNamedArg)
      {
      d->namedArgs[d->multiNamedArg].appendValue(arg);
      }
    else if (pos < d->namedArgs.count())
      {
      d->namedArgs[pos].setValue(arg);
      }
    }

  // If asked, check required options/arguments
  if (!options.testFlag(qtCliArgs::IgnoreRequired))
    {
    if (!d->checkRequired())
      {
      return false;
      }
    }

  // Finalize parsing
  d->generateQtArgs();
  d->parseResult = true;
  return true;
}

//-----------------------------------------------------------------------------
void qtCliArgs::parseOrDie(int exitCode)
{
  this->parseOrDie({}, exitCode);
}

//-----------------------------------------------------------------------------
void qtCliArgs::parseOrDie(ParseOption option, int exitCode)
{
  this->parseOrDie(ParseOptions(option), exitCode);
}

//-----------------------------------------------------------------------------
void qtCliArgs::parseOrDie(ParseOptions options, int exitCode)
{
  if (!this->parse(options))
    {
    this->parseError();
    exit(exitCode);
    }
}

//-----------------------------------------------------------------------------
void qtCliArgs::parseError() const
{
  QTE_D_CONST(qtCliArgs);
  this->usageError(d->parseError);
}

//-----------------------------------------------------------------------------
void qtCliArgs::shortUsage() const
{
  QTE_D_CONST(qtCliArgs);

  // Build options groups list
  QStringList args;
  foreach (auto const& group, d->optionGroups)
    {
    if (!group.isEmpty())
      {
      args.append(QString("[%1-options]").arg(group));
      }
    }
  if (d->optionGroups.contains(""))
    {
    args.append("[options]");
    }

  // Build list of named options
  foreach (auto const& narg, d->namedArgs)
    {
    QString format;
    if (narg.isRequired())
      {
      format = (narg.isNamedList() ? "%1 [%1...]" : "%1");
      }
    else
      {
      format = (narg.isNamedList() ? "[%1...]" : "[%1]");
      }
    args.append(format.arg(narg.preferredName()));
    }

  // Show short usage
  int ghIndent = 8 + d->processName.length();
  QString argsText = qtCliArgs::joinWrapped(args, ghIndent, ghIndent);
  qOut() << "Usage: " << d->processName << ' ' << argsText << "\n";
}

//-----------------------------------------------------------------------------
void qtCliArgs::usage(QString group) const
{
  QTE_D_CONST(qtCliArgs);

  // Set up options list for built-in options
  qtCliOptions genericOptions;
  genericOptions.add("help", "Show help for common options");

  // Build help options for generic help
  foreach (auto const& group, d->optionGroups)
    {
    if (!group.isEmpty())
      {
      genericOptions.add(QString("help-%1").arg(group.toLower()),
                         QString("Show %1 specific options").arg(group));
      }
    }

  // Add remaining generic options
  genericOptions.add("help-all", "Show all options");
  genericOptions.add("v").add("version", "Show version information");
  genericOptions.add("-", "End of options");

  // Show short usage and help for generic options
  this->shortUsage();
  qOut() << "\nGeneric options:\n";
  foreach (auto const& option, genericOptions.options())
    qOut() << option.optionUsage() << '\n';

  // Show help for requested group(s)
  if (!group.isEmpty() && group != "all")
    {
    const QString lgroup = group.toLower();
    if (d->options.contains(lgroup))
      {
      const qtCliArgsPrivate::OptionGroup& og = d->options[lgroup];

      // Show header
      qOut() << '\n' << og.name << " options:\n";

      // Show options for this group
      foreach (auto const& option, og.options)
        qOut() << option.optionUsage() << '\n';
      }
    }
  else
    {
    // Show help for basic/all group(s)
    QStringList groups =
      (group == "all" ? d->optionGroups : d->commonOptionGroups);
    foreach (auto const& groupName, groups)
      {
      const QString lgn = groupName.toLower();
      const qtCliArgsPrivate::OptionGroup& og = d->options[lgn];

      // Show header
      if (groupName.isEmpty())
        {
        qOut() << "\nOptions:\n";
        }
      else
        {
        qOut() << '\n' << og.name << " options:\n";
        }

      // Show options for this group
      foreach (auto const& option, og.options)
        qOut() << option.optionUsage() << '\n';
      }

    // Show help for named arguments
    if (!d->namedArgs.isEmpty())
      {
      // Show header
      qOut() << "\nArguments:\n";

      // Show named arguments
      foreach (auto const& narg, d->namedArgs)
        qOut() << narg.namedArgUsage() << '\n';
      }
    }
}

//-----------------------------------------------------------------------------
void qtCliArgs::usageError(const QString& message) const
{
  QTE_D_CONST(qtCliArgs);
  const QString prefix = d->processName + ": ";
  qErr() << prefix << message << '\n' << prefix
         << "Use --help to get a list of available command line options.\n";
}

//-----------------------------------------------------------------------------
bool qtCliArgs::isSet(const QString& name) const
{
  QTE_D_CONST(qtCliArgs);
  qtCliOption* option = d->optionMap.value(name, 0);
  if (option)
    {
    return option->isSet();
    }
  else if (name.length() > 4 && name.startsWith("no-"))
    {
    qtCliOption* option = d->optionMap.value(name.mid(3), 0);
    return (option ? !option->isSet() : false);
    }
  else
    {
    qtCliOption* option = d->optionMap.value("no-" + name, 0);
    return (option ? !option->isSet() : false);
    }
}

//-----------------------------------------------------------------------------
QString qtCliArgs::value(const QString& name) const
{
  QTE_D_CONST(qtCliArgs);
  qtCliOption* option = d->optionMap.value(name, 0);
  return (option ? option->value() : QString());
}

//-----------------------------------------------------------------------------
QStringList qtCliArgs::values(const QString& name) const
{
  QTE_D_CONST(qtCliArgs);
  qtCliOption* option = d->optionMap.value(name, 0);
  return (option ? option->values() : QStringList());
}

//-----------------------------------------------------------------------------
int qtCliArgs::count() const
{
  QTE_D_CONST(qtCliArgs);
  return d->parsedArgs.count();
}

//-----------------------------------------------------------------------------
QString qtCliArgs::arg(int index) const
{
  QTE_D_CONST(qtCliArgs);
  return d->parsedArgs.at(index);
}

//-----------------------------------------------------------------------------
QStringList qtCliArgs::args() const
{
  QTE_D_CONST(qtCliArgs);
  return d->parsedArgs;
}

//-----------------------------------------------------------------------------
QString qtCliArgs::executableName() const
{
  QTE_D_CONST(qtCliArgs);
  return d->processName;
}

//-----------------------------------------------------------------------------
int& qtCliArgs::qtArgc()
{
  QTE_D(qtCliArgs);

  // Create new mutable argc
  int* argc = new int(d->qtArgs.count());

  // Add to bookkeeping (for later deletion) and return
  d->qtArgc.append(argc);
  return *argc;
}

//-----------------------------------------------------------------------------
char** qtCliArgs::qtArgv()
{
  QTE_D(qtCliArgs);

  // Build new mutable argv vector
  int k = d->qtArgs.count();
  char** argv = new char*[k + 1];
  argv[k] = 0;
  while (k--)
    {
    QByteArray arg = d->qtArgs[k].toLocal8Bit();
    auto al = static_cast<size_t>(arg.size());
    char* parg = new char[al + 1];
    memcpy(parg, arg.constData(), sizeof(char) * (al + 1));
    argv[k] = parg;
    }

  // Add to bookkeeping (for later deletion) and return
  d->qtArgv.append(argv);
  return argv;
}

//-----------------------------------------------------------------------------
QString qtCliArgs::joinWrapped(QStringList words, int indent, int prefixSize)
{
  // Try to get tty width (really, best guess based on hoping $COLUMNS is set)
  int columns = qgetenv("COLUMNS").toInt();
  if (!columns)
    {
    columns = 80;
    }
#ifdef Q_OS_WIN
  // Unlike better terminals, the Windows default console does not suppress a
  // newline following a full line of text, which results in spurious blank
  // lines; suppress this by not using the last column for text
  --columns;
#endif

  // Join description words to wrapped text
  QString result;
  int remaining = columns - prefixSize;
  while (words.count())
    {
    const int wl = words.first().length();
    if ((wl + (result.isEmpty() ? 0 : 1)) > remaining)
      {
      result += '\n' + QString(indent, ' ');
      remaining = columns - indent;
      }
    else if (!result.isEmpty())
      {
      result += ' ';
      --remaining;
      }
    result += words.takeFirst();
    remaining -= wl;
    }

  // Return wrapped text
  return result;
}

//END qtCliArgs
