/* Copyright (c) 2018, Google Inc.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE. */

#include "./wycheproof_util_reduced.h"

#include <limits.h>
#include <stdlib.h>

#include <algorithm>

#include "../crypto/test/file_test.h"


bool WycheproofResult_Reduced::IsValid(
    const std::vector<std::string> &acceptable_flags) const {
  switch (raw_result) {
    case WycheproofRawResult_Reduced::kValid:
      return true;
    case WycheproofRawResult_Reduced::kInvalid:
      return false;
    case WycheproofRawResult_Reduced::kAcceptable:
      for (const auto &flag : flags) {
        if (std::find(acceptable_flags.begin(), acceptable_flags.end(), flag) ==
            acceptable_flags.end()) {
          return false;
        }
      }
      return true;
  }

  abort();
}

bool GetWycheproofResult_Reduced(FileTest *t, WycheproofResult_Reduced *out) {
  std::string result;
  if (!t->GetAttribute(&result, "result")) {
    return false;
  }
  if (result == "valid") {
    out->raw_result = WycheproofRawResult_Reduced::kValid;
  } else if (result == "invalid") {
    out->raw_result = WycheproofRawResult_Reduced::kInvalid;
  } else if (result == "acceptable") {
    out->raw_result = WycheproofRawResult_Reduced::kAcceptable;
  } else {
    t->PrintLine("Bad result string '%s'", result.c_str());
    return false;
  }

  out->flags.clear();
  if (t->HasAttribute("flags")) {
    std::string flags = t->GetAttributeOrDie("flags");
    size_t idx = 0;
    while (idx < flags.size()) {
      size_t comma = flags.find(',', idx);
      if (comma == std::string::npos) {
        comma = flags.size();
      }
      out->flags.push_back(flags.substr(idx, comma - idx));
      idx = comma + 1;
    }
  }
  return true;
}
