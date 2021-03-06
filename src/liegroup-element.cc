// Copyright (c) 2017, CNRS
// Authors: Florent Lamiraux
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

#include <hpp/pinocchio/liegroup-element.hh>
#include "../src/size-visitor.hh"
#include "../src/addition-visitor.hh"
#include "../src/substraction-visitor.hh"
#include "../src/log-visitor.hh"

namespace hpp {
  namespace pinocchio {
    typedef std::vector <LiegroupType> LiegroupTypes;

    void LiegroupElement::setNeutral ()
    {
      value_ = space_->neutral ().vector ();
    }

    LiegroupElement operator+ (const LiegroupElement& e, const vector_t& v)
    {
      assert (e.space ()->nv () == v.size ());
      typedef std::vector <LiegroupType> LiegroupTypes;
      LiegroupElement result (e);
      size_type iq = 0, iv = 0;
      for (LiegroupTypes::const_iterator it =
             e.space ()->liegroupTypes ().begin ();
           it != e.space ()->liegroupTypes ().end (); ++it) {
        liegroupType::SizeVisitor dv;
        boost::apply_visitor (dv, *it);

        liegroupType::AdditionVisitor av (e.value_.segment (iq, dv.nq),
                                          v.segment (iv, dv.nv));
        boost::apply_visitor (av, *it);
        result.value_.segment (iq, dv.nq) = av.result;
        iq += dv.nq;
        iv += dv.nv;
      }
      return result;
    }

    vector_t operator- (const LiegroupElement& e1, const LiegroupElement& e2)
    {
      assert (e1.space ()->nq () == e2.space ()->nq ());
      typedef std::vector <LiegroupType> LiegroupTypes;
      vector_t result (e1.space ()->nv ());
      size_type iq = 0, iv = 0;

      LiegroupTypes::const_iterator it1 =
        e1.space ()->liegroupTypes ().begin ();
      LiegroupTypes::const_iterator it2 =
        e2.space ()->liegroupTypes ().begin ();

      while ((it1 != e1.space ()->liegroupTypes ().end ()) &&
             (it2 != e2.space ()->liegroupTypes ().end ())) {
        liegroupType::SizeVisitor dv;
        boost::apply_visitor (dv, *it1);

        liegroupType::SubstractionVisitor sv (e1.value_.segment (iq, dv.nq),
                                              e2.value_.segment (iq, dv.nq),
                                              e1.space ()->nv ());
        boost::apply_visitor (sv, *it1);
        result.segment (iv, dv.nv) = sv.result;
        iq += dv.nq;
        iv += dv.nv;
        ++it1; ++it2;
      }
      return result;
    }

    vector_t log (const LiegroupElement& lge)
    {
      using liegroupType::LogVisitor;
      vector_t res (lge.space ()->nv ());
      size_type iq = 0, iv = 0;
      for (LiegroupTypes::const_iterator it =
           lge.space ()->liegroupTypes ().begin ();
           it != lge.space ()->liegroupTypes ().end (); ++it) {
        liegroupType::SizeVisitor sizeVisitor;
        boost::apply_visitor (sizeVisitor, *it);
        LogVisitor logVisitor (lge.vector ().segment (iq, sizeVisitor.nq),
                               res.segment (iv, sizeVisitor.nv));
        boost::apply_visitor (logVisitor, *it);
      }
      return res;
    }
  } // namespace pinocchio
} // namespace hpp
