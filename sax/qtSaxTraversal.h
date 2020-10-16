// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

#ifndef __qtSaxTraversal_h
#define __qtSaxTraversal_h

/// \file

#include <qtExports.h>

#include <QXmlStreamReader>

/// Streaming XML traversal helper class.
///
/// This class is used to traverse the child nodes of an XML stream. It is used
/// in the implementation of #foreach_sax_child and should not normally be used
/// directly.
class QTE_EXPORT qtSaxTraversal
{
public:
  /// Construct traversal helper.
  ///
  /// This constructs a traversal helper for \p stream. The traversal begins at
  /// the current token (normally a QXmlStreamReader::StartElement) and ends
  /// when the node stack unwinds past its starting point.
  explicit qtSaxTraversal(QXmlStreamReader& stream);

  /// Get inner iteration flag.
  ///
  /// \return \c true if inner iteration should proceed, otherwise \c false.
  inline operator bool() { return this->Depth > 0 && this->Flag; }

  /// Begin iteration step.
  ///
  /// This begins an iteration step by setting the inner iteration flag to
  /// \c true.
  ///
  /// \return \c true if iteration may proceed (i.e. the traversal remains
  ///         within the scope in which it was started), otherwise \c false.
  inline bool begin() { this->Flag = true; return this->Depth > 0; }

  /// End iteration step.
  ///
  /// This ends the current iteration step, making the inner iteration flag
  /// \c false.
  inline void end() { this->Flag = false; }

  /// Obtain the next token to be processed.
  ///
  /// This obtains the next token to be processed. If the stream position has
  /// changed since the previous call to next() (or since construction, if
  /// next() has not yet been called on this instance), then the current token
  /// is processed. Otherwise QXmlStreamReader::readNext() is called to obtain
  /// the next token.
  QXmlStreamReader::TokenType next();

private:
  QXmlStreamReader& Stream;
  int Depth;
  bool Flag;
  qint64 Offset;
};

/// Alias for #foreach_sax_child.
///
/// This macro is a synonym for #foreach_sax_child that is guaranteed to be
/// available. (If QT_NO_KEYWORDS is defined, the more convenient
/// #foreach_sax_child is hidden to avoid namespace pollution.)
#define QTE_FOREACH_SAX_CHILD(decl, stream) \
  for (qtSaxTraversal __t(stream); __t.begin();) \
    for (decl = __t.next(); __t; __t.end())

#ifdef DOXYGEN
/// Iterate over XML child nodes.
///
/// This macro iterates over the child nodes of the current XML node, stopping
/// when the current node ends. The end of the current node is not processed,
/// however it may normally be assumed that, when the loop exits, the XML
/// stream points to the end of the node at which iteration was started.
///
/// It is safe to nest foreach_sax_child loops. However, care must be taken
/// when calling QXmlStreamReader::readNext() from within such a loop to avoid
/// corrupting the iteration state. Additionally, it is recommended to only
/// begin iteration when the current token is a QXmlStreamReader::StartElement.
///
/// \param decl
///   Name or declaration of variable that receives the XML token for each
///   iteration step.
/// \param stream QXmlStreamReader over which to iterate.
///
/// \par Example:
/// \code{.cpp}
/// readFooElement(QXmlStreamReader& stream)
/// {
///   foreach_sax_child (const QXmlStreamReader::TokenType token, stream)
///   {
///     // Do something with the child tokens...
///   }
/// }
/// \endcode
#  define foreach_sax_child(decl, stream)
#else
#  ifndef QT_NO_KEYWORDS
#    ifndef foreach_sax_child
#      define foreach_sax_child QTE_FOREACH_SAX_CHILD
#    endif
#  endif
#endif

#endif
