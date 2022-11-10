#pragma once
/* 
 * parser.h --- 
 * 
 * Author: Stephen Taylor
 * Created: 11-01-2020
 * Version: 1.0
 * 
 * Description: 
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <hammer/hammer.h>
#include <hammer/glue.h>
#include <hammer/test_suite.h>

#include <glib.h>

HParser *init_parser();
