// Copyright (c) 2017, Joseph Mirabel
// Authors: Joseph Mirabel (joseph.mirabel@laas.fr)
//
// This file is part of hpp-pinocchio.
// hpp-pinocchio is free software: you can redistribute it
// and/or modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation, either version
// 3 of the License, or (at your option) any later version.
//
// hpp-pinocchio is distributed in the hope that it will be
// useful, but WITHOUT ANY WARRANTY; without even the implied warranty
// of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Lesser Public License for more details.  You should have
// received a copy of the GNU Lesser General Public License along with
// hpp-pinocchio. If not, see <http://www.gnu.org/licenses/>.

#ifndef HPP_PINOCCHIO_LIEGROUP_HH
#define HPP_PINOCCHIO_LIEGROUP_HH

#include <pinocchio/multibody/joint/fwd.hpp>
// #include <pinocchio/multibody/liegroup/liegroup.hpp>

#include <hpp/pinocchio/liegroup/vector-space.hh>
#include <hpp/pinocchio/liegroup/cartesian-product.hh>
#include <hpp/pinocchio/liegroup/special-orthogonal.hh>

namespace hpp {
  namespace pinocchio {
    // Default implementation is empty
    struct LieGroupTpl {
      template<typename JointModel> struct operation {};
    };
    // JointModelRevolute, JointModelRevoluteUnbounded, JointModelRevoluteUnaligned
    template<int Axis> struct LieGroupTpl::operation <se3::JointModelRevolute<Axis> > {
      typedef liegroup::VectorSpaceOperation<1, true> type;
    };
    template<int Axis> struct LieGroupTpl::operation <se3::JointModelRevoluteUnbounded<Axis> > {
      typedef liegroup::SpecialOrthogonalOperation<2> type;
    };
    template<> struct LieGroupTpl::operation <se3::JointModelRevoluteUnaligned > {
      typedef liegroup::VectorSpaceOperation<1, true> type;
    };

    // JointModelPrismatic, JointModelPrismaticUnaligned, JointModelTranslation
    template<int Axis> struct LieGroupTpl::operation <se3::JointModelPrismatic<Axis> > {
      typedef liegroup::VectorSpaceOperation<1, false> type;
    };
    template<> struct LieGroupTpl::operation <se3::JointModelPrismaticUnaligned > {
      typedef liegroup::VectorSpaceOperation<1, false> type;
    };
    template<> struct LieGroupTpl::operation <se3::JointModelTranslation > {
      typedef liegroup::VectorSpaceOperation<3, false> type;
    };

    // JointModelSpherical, JointModelSphericalZYX,
    template<> struct LieGroupTpl::operation <se3::JointModelSpherical> {
      typedef liegroup::SpecialOrthogonalOperation<3> type;
    };
    template<> struct LieGroupTpl::operation <se3::JointModelSphericalZYX> {
      typedef liegroup::VectorSpaceOperation<3, true> type;
    };

    // JointModelFreeFlyer, JointModelPlanar
    template<> struct LieGroupTpl::operation <se3::JointModelFreeFlyer> {
      typedef liegroup::CartesianProductOperation<
        liegroup::VectorSpaceOperation<3, false>,
        liegroup::SpecialOrthogonalOperation<3>
        > type;
    };
    template<> struct LieGroupTpl::operation <se3::JointModelPlanar> {
      typedef liegroup::CartesianProductOperation<
        liegroup::VectorSpaceOperation<2, false>,
        liegroup::SpecialOrthogonalOperation<2>
        > type;
    };
  } // namespace pinocchio
} // namespace hpp

#endif // HPP_PINOCCHIO_LIEGROUP_HH
