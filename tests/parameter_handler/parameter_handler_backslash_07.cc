// ------------------------------------------------------------------------
//
// SPDX-License-Identifier: LGPL-2.1-or-later
// Copyright (C) 2016 - 2018 by the deal.II authors
//
// This file is part of the deal.II library.
//
// Part of the source code is dual licensed under Apache-2.0 WITH
// LLVM-exception OR LGPL-2.1-or-later. Detailed license information
// governing the source code and code contributions can be found in
// LICENSE.md and CONTRIBUTING.md at the top level directory of deal.II.
//
// ------------------------------------------------------------------------


#include <deal.II/base/parameter_handler.h>

#include "../tests.h"

/*
 * If a parameter file line ends in a '\', then the whitespace at at the
 * beginning of the next line is ignored when joining the lines. For example,
 * the input
 *
 *      set value = val\
 *                  u\
 *                  e
 *
 * is parsed as 'set value = value'.
 */


int
main()
{
  initlog();

  for (unsigned int i = 0; i < 2; ++i)
    {
      ParameterHandler prm;
      prm.enter_subsection("Testing");
      prm.declare_entry("value", "value", Patterns::Anything());
      prm.leave_subsection();

      // test both relevant parse_input functions
      if (i == 0)
        {
          prm.parse_input(SOURCE_DIR "/prm/parameter_handler_backslash_07.prm");
        }
      else
        {
          std::ifstream input_stream(SOURCE_DIR
                                     "/prm/parameter_handler_backslash_07.prm");
          prm.parse_input(input_stream);
        }

      std::string list;
      prm.enter_subsection("Testing");
      list = prm.get("value");
      prm.leave_subsection();

      deallog << list << std::endl;
    }

  return 0;
}
