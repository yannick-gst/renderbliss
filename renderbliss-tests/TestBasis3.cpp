// Copyright (c) 2008-2011 Yannick Tapsoba.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#include <UnitTest++.h>
#include <boost/random.hpp>
#include "renderbliss/Math/MathUtils.h"
#include "renderbliss/Math/Geometry/Basis3.h"

namespace
{
boost::mt19937 rng;
boost::uniform_real<renderbliss::real> dist(-225, 225);
boost::variate_generator<boost::mt19937, boost::uniform_real<renderbliss::real> > die(rng, dist);

struct Basis3Fixture
{
    renderbliss::Vector3 u, v, n;
    renderbliss::Basis3 basis;

    Basis3Fixture() :   u(die(), die(), die()),
                        v(die(), die(), die()),
                        n(renderbliss::CrossProduct(u,v)) {}
};

TEST_FIXTURE(Basis3Fixture, CheckSetFromU)
{
    basis = renderbliss::Basis3::CreateFromU(u);

    renderbliss::Vector3 unitU = u.GetNormalized();

    CHECK_CLOSE(unitU.x, basis.U().x, renderbliss::Epsilon());
    CHECK_CLOSE(unitU.y, basis.U().y, renderbliss::Epsilon());
    CHECK_CLOSE(unitU.z, basis.U().z, renderbliss::Epsilon());

    CHECK_CLOSE(0, renderbliss::DotProduct(basis.U(), basis.V()), renderbliss::Epsilon());
    CHECK_CLOSE(0, renderbliss::DotProduct(basis.V(), basis.N()), renderbliss::Epsilon());
    CHECK_CLOSE(0, renderbliss::DotProduct(basis.N(), basis.U()), renderbliss::Epsilon());

    renderbliss::Vector3 u_cross_v = renderbliss::CrossProduct(basis.U(), basis.V());
    CHECK_CLOSE(basis.N().x, u_cross_v.x, renderbliss::Epsilon());
    CHECK_CLOSE(basis.N().y, u_cross_v.y, renderbliss::Epsilon());
    CHECK_CLOSE(basis.N().z, u_cross_v.z, renderbliss::Epsilon());

    renderbliss::Vector3 v_cross_n = renderbliss::CrossProduct(basis.V(), basis.N());
    CHECK_CLOSE(basis.U().x, v_cross_n.x, renderbliss::Epsilon());
    CHECK_CLOSE(basis.U().y, v_cross_n.y, renderbliss::Epsilon());
    CHECK_CLOSE(basis.U().z, v_cross_n.z, renderbliss::Epsilon());

    renderbliss::Vector3 n_cross_u = renderbliss::CrossProduct(basis.N(), basis.U());
    CHECK_CLOSE(basis.V().x, n_cross_u.x, renderbliss::Epsilon());
    CHECK_CLOSE(basis.V().y, n_cross_u.y, renderbliss::Epsilon());
    CHECK_CLOSE(basis.V().z, n_cross_u.z, renderbliss::Epsilon());
}

TEST_FIXTURE(Basis3Fixture, CheckSetFromV)
{
    basis = renderbliss::Basis3::CreateFromV(v);

    renderbliss::Vector3 unitV = v.GetNormalized();

    CHECK_CLOSE(unitV.x, basis.V().x, renderbliss::Epsilon());
    CHECK_CLOSE(unitV.y, basis.V().y, renderbliss::Epsilon());
    CHECK_CLOSE(unitV.z, basis.V().z, renderbliss::Epsilon());

    CHECK_CLOSE(0, renderbliss::DotProduct(basis.U(), basis.V()), renderbliss::Epsilon());
    CHECK_CLOSE(0, renderbliss::DotProduct(basis.V(), basis.N()), renderbliss::Epsilon());
    CHECK_CLOSE(0, renderbliss::DotProduct(basis.N(), basis.U()), renderbliss::Epsilon());

    renderbliss::Vector3 u_cross_v = renderbliss::CrossProduct(basis.U(), basis.V());
    CHECK_CLOSE(basis.N().x, u_cross_v.x, renderbliss::Epsilon());
    CHECK_CLOSE(basis.N().y, u_cross_v.y, renderbliss::Epsilon());
    CHECK_CLOSE(basis.N().z, u_cross_v.z, renderbliss::Epsilon());

    renderbliss::Vector3 v_cross_n = renderbliss::CrossProduct(basis.V(), basis.N());
    CHECK_CLOSE(basis.U().x, v_cross_n.x, renderbliss::Epsilon());
    CHECK_CLOSE(basis.U().y, v_cross_n.y, renderbliss::Epsilon());
    CHECK_CLOSE(basis.U().z, v_cross_n.z, renderbliss::Epsilon());

    renderbliss::Vector3 n_cross_u = renderbliss::CrossProduct(basis.N(), basis.U());
    CHECK_CLOSE(basis.V().x, n_cross_u.x, renderbliss::Epsilon());
    CHECK_CLOSE(basis.V().y, n_cross_u.y, renderbliss::Epsilon());
    CHECK_CLOSE(basis.V().z, n_cross_u.z, renderbliss::Epsilon());
}

TEST_FIXTURE(Basis3Fixture, CheckSetFromN)
{
    basis = renderbliss::Basis3::CreateFromN(n);

    renderbliss::Vector3 unitN = n.GetNormalized();

    CHECK_CLOSE(unitN.x, basis.N().x, renderbliss::Epsilon());
    CHECK_CLOSE(unitN.y, basis.N().y, renderbliss::Epsilon());
    CHECK_CLOSE(unitN.z, basis.N().z, renderbliss::Epsilon());

    CHECK_CLOSE(0, renderbliss::DotProduct(basis.U(), basis.V()), renderbliss::Epsilon());
    CHECK_CLOSE(0, renderbliss::DotProduct(basis.V(), basis.N()), renderbliss::Epsilon());
    CHECK_CLOSE(0, renderbliss::DotProduct(basis.N(), basis.U()), renderbliss::Epsilon());

    renderbliss::Vector3 u_cross_v = renderbliss::CrossProduct(basis.U(), basis.V());
    CHECK_CLOSE(basis.N().x, u_cross_v.x, renderbliss::Epsilon());
    CHECK_CLOSE(basis.N().y, u_cross_v.y, renderbliss::Epsilon());
    CHECK_CLOSE(basis.N().z, u_cross_v.z, renderbliss::Epsilon());

    renderbliss::Vector3 v_cross_n = renderbliss::CrossProduct(basis.V(), basis.N());
    CHECK_CLOSE(basis.U().x, v_cross_n.x, renderbliss::Epsilon());
    CHECK_CLOSE(basis.U().y, v_cross_n.y, renderbliss::Epsilon());
    CHECK_CLOSE(basis.U().z, v_cross_n.z, renderbliss::Epsilon());

    renderbliss::Vector3 n_cross_u = renderbliss::CrossProduct(basis.N(), basis.U());
    CHECK_CLOSE(basis.V().x, n_cross_u.x, renderbliss::Epsilon());
    CHECK_CLOSE(basis.V().y, n_cross_u.y, renderbliss::Epsilon());
    CHECK_CLOSE(basis.V().z, n_cross_u.z, renderbliss::Epsilon());
}

TEST_FIXTURE(Basis3Fixture, CheckSetFromUV)
{
    basis = renderbliss::Basis3::CreateFromUV(u, v);

    renderbliss::Vector3 unitN = renderbliss::CrossProduct(u,v).GetNormalized();
    CHECK_CLOSE(unitN.x, basis.N().x, renderbliss::Epsilon());
    CHECK_CLOSE(unitN.y, basis.N().y, renderbliss::Epsilon());
    CHECK_CLOSE(unitN.z, basis.N().z, renderbliss::Epsilon());

    CHECK_CLOSE(0, renderbliss::DotProduct(basis.U(), basis.V()), renderbliss::Epsilon());
    CHECK_CLOSE(0, renderbliss::DotProduct(basis.V(), basis.N()), renderbliss::Epsilon());
    CHECK_CLOSE(0, renderbliss::DotProduct(basis.N(), basis.U()), renderbliss::Epsilon());

    renderbliss::Vector3 u_cross_v = renderbliss::CrossProduct(basis.U(), basis.V());
    CHECK_CLOSE(basis.N().x, u_cross_v.x, renderbliss::Epsilon());
    CHECK_CLOSE(basis.N().y, u_cross_v.y, renderbliss::Epsilon());
    CHECK_CLOSE(basis.N().z, u_cross_v.z, renderbliss::Epsilon());

    renderbliss::Vector3 v_cross_n = renderbliss::CrossProduct(basis.V(), basis.N());
    CHECK_CLOSE(basis.U().x, v_cross_n.x, renderbliss::Epsilon());
    CHECK_CLOSE(basis.U().y, v_cross_n.y, renderbliss::Epsilon());
    CHECK_CLOSE(basis.U().z, v_cross_n.z, renderbliss::Epsilon());

    renderbliss::Vector3 n_cross_u = renderbliss::CrossProduct(basis.N(), basis.U());
    CHECK_CLOSE(basis.V().x, n_cross_u.x, renderbliss::Epsilon());
    CHECK_CLOSE(basis.V().y, n_cross_u.y, renderbliss::Epsilon());
    CHECK_CLOSE(basis.V().z, n_cross_u.z, renderbliss::Epsilon());
}

TEST_FIXTURE(Basis3Fixture, CheckSetFromVN)
{
    basis = renderbliss::Basis3::CreateFromVN(v, n);

    renderbliss::Vector3 unitU = renderbliss::CrossProduct(v,n).GetNormalized();
    CHECK_CLOSE(unitU.x, basis.U().x, renderbliss::Epsilon());
    CHECK_CLOSE(unitU.y, basis.U().y, renderbliss::Epsilon());
    CHECK_CLOSE(unitU.z, basis.U().z, renderbliss::Epsilon());

    CHECK_CLOSE(0, renderbliss::DotProduct(basis.U(), basis.V()), renderbliss::Epsilon());
    CHECK_CLOSE(0, renderbliss::DotProduct(basis.V(), basis.N()), renderbliss::Epsilon());
    CHECK_CLOSE(0, renderbliss::DotProduct(basis.N(), basis.U()), renderbliss::Epsilon());

    renderbliss::Vector3 u_cross_v = renderbliss::CrossProduct(basis.U(), basis.V());
    CHECK_CLOSE(basis.N().x, u_cross_v.x, renderbliss::Epsilon());
    CHECK_CLOSE(basis.N().y, u_cross_v.y, renderbliss::Epsilon());
    CHECK_CLOSE(basis.N().z, u_cross_v.z, renderbliss::Epsilon());

    renderbliss::Vector3 v_cross_n = renderbliss::CrossProduct(basis.V(), basis.N());
    CHECK_CLOSE(basis.U().x, v_cross_n.x, renderbliss::Epsilon());
    CHECK_CLOSE(basis.U().y, v_cross_n.y, renderbliss::Epsilon());
    CHECK_CLOSE(basis.U().z, v_cross_n.z, renderbliss::Epsilon());

    renderbliss::Vector3 n_cross_u = renderbliss::CrossProduct(basis.N(), basis.U());
    CHECK_CLOSE(basis.V().x, n_cross_u.x, renderbliss::Epsilon());
    CHECK_CLOSE(basis.V().y, n_cross_u.y, renderbliss::Epsilon());
    CHECK_CLOSE(basis.V().z, n_cross_u.z, renderbliss::Epsilon());
}

TEST_FIXTURE(Basis3Fixture, CheckSetFromNU)
{
    basis = renderbliss::Basis3::CreateFromNU(n, u);

    renderbliss::Vector3 unitV = renderbliss::CrossProduct(n,u).GetNormalized();
    CHECK_CLOSE(unitV.x, basis.V().x, renderbliss::Epsilon());
    CHECK_CLOSE(unitV.y, basis.V().y, renderbliss::Epsilon());
    CHECK_CLOSE(unitV.z, basis.V().z, renderbliss::Epsilon());

    CHECK_CLOSE(0, renderbliss::DotProduct(basis.U(), basis.V()), renderbliss::Epsilon());
    CHECK_CLOSE(0, renderbliss::DotProduct(basis.V(), basis.N()), renderbliss::Epsilon());
    CHECK_CLOSE(0, renderbliss::DotProduct(basis.N(), basis.U()), renderbliss::Epsilon());

    renderbliss::Vector3 u_cross_v = renderbliss::CrossProduct(basis.U(), basis.V());
    CHECK_CLOSE(basis.N().x, u_cross_v.x, renderbliss::Epsilon());
    CHECK_CLOSE(basis.N().y, u_cross_v.y, renderbliss::Epsilon());
    CHECK_CLOSE(basis.N().z, u_cross_v.z, renderbliss::Epsilon());

    renderbliss::Vector3 v_cross_n = renderbliss::CrossProduct(basis.V(), basis.N());
    CHECK_CLOSE(basis.U().x, v_cross_n.x, renderbliss::Epsilon());
    CHECK_CLOSE(basis.U().y, v_cross_n.y, renderbliss::Epsilon());
    CHECK_CLOSE(basis.U().z, v_cross_n.z, renderbliss::Epsilon());

    renderbliss::Vector3 n_cross_u = renderbliss::CrossProduct(basis.N(), basis.U());
    CHECK_CLOSE(basis.V().x, n_cross_u.x, renderbliss::Epsilon());
    CHECK_CLOSE(basis.V().y, n_cross_u.y, renderbliss::Epsilon());
    CHECK_CLOSE(basis.V().z, n_cross_u.z, renderbliss::Epsilon());
}
}
