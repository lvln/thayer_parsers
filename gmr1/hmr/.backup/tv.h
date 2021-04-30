#pragma once

#include <stdio.h>
#include <string.h>
#include <hammer/hammer.h>
#include <hammer/glue.h>
#include <hammer/test_suite.h>

#include <glib.h>

void register_tests(const HParser *parser);
