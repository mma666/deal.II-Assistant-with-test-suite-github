// ------------------------------------------------------------------------
//
// SPDX-License-Identifier: LGPL-2.1-or-later
// Copyright (C) 2004 - 2023 by the deal.II authors
//
// This file is part of the deal.II library.
//
// Part of the source code is dual licensed under Apache-2.0 WITH
// LLVM-exception OR LGPL-2.1-or-later. Detailed license information
// governing the source code and code contributions can be found in
// LICENSE.md and CONTRIBUTING.md at the top level directory of deal.II.
//
// ------------------------------------------------------------------------



// verify that VectorBase::print uses the precision parameter correctly and
// restores the previous value of the stream precision

#include <deal.II/lac/petsc_vector.h>

#include <iostream>
#include <vector>

#include "../tests.h"


int
main(int argc, char **argv)
{
  initlog();

  try
    {
      Utilities::MPI::MPI_InitFinalize mpi_initialization(argc, argv, 1);
      {
        IndexSet indices(5);
        indices.add_range(0, 5);
        PETScWrappers::MPI::Vector v(indices, MPI_COMM_WORLD);
        for (unsigned int i = 0; i < v.size(); ++i)
          v(i) = i * 1.2345678901234567;

        // print with old precision
        deallog << numbers::PI << std::endl;

        // print with prescribed precision
        deallog << "across=false" << std::endl;
        v.print(deallog.get_file_stream(), 10, true, false);

        deallog << "across=true" << std::endl;
        v.print(deallog.get_file_stream(), 10, true, true);

        // print once more. should be the old precision again
        deallog << numbers::PI << std::endl;
      }
    }
  catch (const std::exception &exc)
    {
      std::cerr << std::endl
                << std::endl
                << "----------------------------------------------------"
                << std::endl;
      std::cerr << "Exception on processing: " << std::endl
                << exc.what() << std::endl
                << "Aborting!" << std::endl
                << "----------------------------------------------------"
                << std::endl;

      return 1;
    }
  catch (...)
    {
      std::cerr << std::endl
                << std::endl
                << "----------------------------------------------------"
                << std::endl;
      std::cerr << "Unknown exception!" << std::endl
                << "Aborting!" << std::endl
                << "----------------------------------------------------"
                << std::endl;
      return 1;
    };
}
