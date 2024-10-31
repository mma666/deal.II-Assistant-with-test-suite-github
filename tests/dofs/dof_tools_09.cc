// ------------------------------------------------------------------------
//
// SPDX-License-Identifier: LGPL-2.1-or-later
// Copyright (C) 2003 - 2022 by the deal.II authors
//
// This file is part of the deal.II library.
//
// Part of the source code is dual licensed under Apache-2.0 WITH
// LLVM-exception OR LGPL-2.1-or-later. Detailed license information
// governing the source code and code contributions can be found in
// LICENSE.md and CONTRIBUTING.md at the top level directory of deal.II.
//
// ------------------------------------------------------------------------


#include "../tests.h"

#include "dof_tools_common.h"

// check
//   DoFTools::map_dof_to_boundary_indices(const DoFHandler<int>     &,
//                                         const std::set<types::boundary_id> &,
//                                         std::vector<unsigned int> &)



template <int dim>
void
check_this(const DoFHandler<dim> &dof_handler)
{
  std::vector<types::global_dof_index> map(dof_handler.n_dofs());
  // check for boundary id 0 alone
  std::set<types::boundary_id> boundary_ids = {0};

  DoFTools::map_dof_to_boundary_indices(dof_handler, boundary_ids, map);
  for (unsigned int i = 0; i < map.size(); ++i)
    deallog << (map[i] == numbers::invalid_dof_index ?
                  -1 :
                  static_cast<signed int>(map[i]))
            << ' ';
  deallog << std::endl;

  // check for boundary id 0 and 1
  boundary_ids.insert(1);
  DoFTools::map_dof_to_boundary_indices(dof_handler, boundary_ids, map);
  for (unsigned int i = 0; i < map.size(); ++i)
    deallog << (map[i] == numbers::invalid_dof_index ?
                  -1 :
                  static_cast<signed int>(map[i]))
            << ' ';
  deallog << std::endl;
}
