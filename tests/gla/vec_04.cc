// ------------------------------------------------------------------------
//
// SPDX-License-Identifier: LGPL-2.1-or-later
// Copyright (C) 2010 - 2020 by the deal.II authors
//
// This file is part of the deal.II library.
//
// Part of the source code is dual licensed under Apache-2.0 WITH
// LLVM-exception OR LGPL-2.1-or-later. Detailed license information
// governing the source code and code contributions can be found in
// LICENSE.md and CONTRIBUTING.md at the top level directory of deal.II.
//
// ------------------------------------------------------------------------



// test assignment of ghost vectors

#include <deal.II/base/index_set.h>

#include <deal.II/lac/affine_constraints.h>
#include <deal.II/lac/generic_linear_algebra.h>

#include <iostream>
#include <vector>

#include "../tests.h"

#include "gla.h"

template <class LA>
void
test()
{
  unsigned int myid    = Utilities::MPI::this_mpi_process(MPI_COMM_WORLD);
  unsigned int numproc = Utilities::MPI::n_mpi_processes(MPI_COMM_WORLD);

  if (myid == 0)
    deallog << "numproc=" << numproc << std::endl;

  // each processor owns 2 indices and all
  // are ghosting Element 1 (the second)

  IndexSet local_active(numproc * 2);
  local_active.add_range(myid * 2, myid * 2 + 2);
  IndexSet local_relevant(numproc * 2);
  local_relevant.add_range(1, 2);

  typename LA::MPI::Vector vb(local_active, MPI_COMM_WORLD);
  typename LA::MPI::Vector v(local_active, local_relevant, MPI_COMM_WORLD);

  vb = 1.0;

  // set local values
  vb(myid * 2)     = myid * 2.0;
  vb(myid * 2 + 1) = myid * 2.0 + 1.0;

  vb.compress(VectorOperation::insert);
  vb *= 2.0;
  v = vb;

  Assert(vb.size() == numproc * 2, ExcInternalError());
  Assert(v.size() == numproc * 2, ExcInternalError());

  Assert(!vb.has_ghost_elements(), ExcInternalError());
  Assert(v.has_ghost_elements(), ExcInternalError());

  typename LA::MPI::Vector test;
  Assert(!test.has_ghost_elements(), ExcInternalError());

  {
    typename LA::MPI::Vector x;
    x = v; // x is empty so it should copy layout(with ghosts) and data
    Assert(x.has_ghost_elements(), ExcInternalError());
    deallog << "ghosted value: " << get_real_assert_zero_imag(x(1))
            << std::endl;
    x = vb; // import, so keep ghost elements
    Assert(x.has_ghost_elements(), ExcInternalError());
    deallog << "ghosted value: " << get_real_assert_zero_imag(x(1))
            << std::endl;
  }
  {
    typename LA::MPI::Vector x;
    x = vb;
    Assert(!x.has_ghost_elements(), ExcInternalError());
    x = test;
    x = v;
    Assert(x.has_ghost_elements(), ExcInternalError());
    deallog << "ghosted value: " << get_real_assert_zero_imag(x(1))
            << std::endl;
  }

  // done
  if (myid == 0)
    deallog << "OK" << std::endl;
}



int
main(int argc, char **argv)
{
  Utilities::MPI::MPI_InitFinalize mpi_initialization(argc, argv, 1);
  MPILogInitAll                    log;
  {
    deallog.push("PETSc");
    test<LA_PETSc>();
    deallog.pop();
    deallog.push("Trilinos");
    test<LA_Trilinos>();
    deallog.pop();
  }
}
