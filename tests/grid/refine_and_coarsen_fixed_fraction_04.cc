// ------------------------------------------------------------------------
//
// SPDX-License-Identifier: LGPL-2.1-or-later
// Copyright (C) 2020 - 2023 by the deal.II authors
//
// This file is part of the deal.II library.
//
// Part of the source code is dual licensed under Apache-2.0 WITH
// LLVM-exception OR LGPL-2.1-or-later. Detailed license information
// governing the source code and code contributions can be found in
// LICENSE.md and CONTRIBUTING.md at the top level directory of deal.II.
//
// ------------------------------------------------------------------------


// Verify fixed fraction algorithm with l1-norm and l2-norm
// Random indicators

#include <deal.II/grid/grid_generator.h>
#include <deal.II/grid/grid_refinement.h>
#include <deal.II/grid/tria.h>

#include <deal.II/lac/vector.h>

#include <limits>

#include "../tests.h"


template <int dim>
void
count_flags(const Triangulation<dim> &tria)
{
  unsigned int n_refine = 0, n_coarsen = 0;
  for (const auto &cell : tria.active_cell_iterators())
    {
      if (cell->refine_flag_set())
        ++n_refine;
      else if (cell->coarsen_flag_set())
        ++n_coarsen;
    }

  deallog << "n_refine_flags: " << n_refine
          << ", n_coarsen_flags: " << n_coarsen;
}



template <int dim>
void
test()
{
  Triangulation<dim> tria;
  GridGenerator::hyper_cube(tria);
  tria.refine_global(2);

  Vector<float> indicator(tria.n_active_cells());
  // assign each cell a globally unique cellid
  for (const auto &cell : tria.active_cell_iterators())
    {
      const std::string  cellid = cell->id().to_string();
      const unsigned int fine_cellid =
        std::stoul(cellid.substr(cellid.find(':') + 1, std::string::npos));

      Testing::srand(fine_cellid);
      indicator[cell->active_cell_index()] = random_value<float>();
    }

  deallog << "l1-norm: ";
  GridRefinement::refine_and_coarsen_fixed_fraction(
    tria,
    indicator,
    0.3,
    0.3,
    std::numeric_limits<unsigned int>::max(),
    VectorTools::L1_norm);
  count_flags(tria);
  deallog << std::endl;

  // reset refinement flags
  for (const auto &cell : tria.active_cell_iterators())
    {
      cell->clear_refine_flag();
      cell->clear_coarsen_flag();
    }

  deallog << "l2-norm: ";
  GridRefinement::refine_and_coarsen_fixed_fraction(
    tria,
    indicator,
    0.3,
    0.3,
    std::numeric_limits<unsigned int>::max(),
    VectorTools::L2_norm);
  count_flags(tria);
  deallog << std::endl;
}



int
main(int argc, const char *argv[])
{
  initlog();

  test<2>();
}
