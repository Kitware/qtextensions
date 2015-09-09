/*ckwg +5
 * Copyright 2013 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#include <QApplication>
#include <QLabel>

#include "../dialogs/qtConfirmationDialog.h"

//-----------------------------------------------------------------------------
int main(int argc, char** argv)
{
  QApplication::setOrganizationName("Kitware");
  QApplication::setApplicationName("Confirmation Dialog Test");
  QApplication app(argc, argv);

  QString prompt =
    "This is a really long confirmation prompt. It needs to be long so that it"
    " will fully test the dialog's handling of size issues and word wrapping"
    " of the prompt.\n\nNotice that there is no check box to suppress this"
    " confirmation; that is because the setting key is blank.\n\nWhen you"
    " select a button, the dialog will go away and the test will end. There"
    " will be no persistent effect either way.";
  qtConfirmationDialog::getConfirmation(0, "Test", prompt);

  return 0;
}
