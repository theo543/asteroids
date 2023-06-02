// This file is compiled as a separate library because on Clang it needs femulated-tls for portable-file-dialogs to work.

#include <portable-file-dialogs.h>

extern void error_box(const std::string& message) {
    pfd::message("Error", message, pfd::choice::ok, pfd::icon::error);
}
