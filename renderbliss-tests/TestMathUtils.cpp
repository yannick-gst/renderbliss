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
#include "renderbliss/Math/MathUtils.h"

namespace
{
TEST(CheckSolveLinearEquation)
{
    // 2x - 6 = 0 (one root)
    renderbliss::real a1 = 2.0f;
    renderbliss::real a0 = -6.0f;
    renderbliss::real root = 0.0f;
    renderbliss::real expectedRoot = 3.0f;
    renderbliss::uint nRoots = renderbliss::SolveLinearEquation(a1, a0, root);
    CHECK_EQUAL(nRoots, static_cast<renderbliss::uint>(1));
    CHECK_CLOSE(expectedRoot, root, renderbliss::Epsilon());

    // 0x - 6 = 0 (no root)
    a1 = 0.0f;
    a0 = -6.0f;
    nRoots = renderbliss::SolveLinearEquation(a1, a0, root);
    CHECK_EQUAL(nRoots, static_cast<renderbliss::uint>(0));
}

TEST(CheckSolveQuadraticEquation)
{
    // 2x^2 - 4x - 3 = 0 (two roots)
    renderbliss::real a2 = 2.0f;
    renderbliss::real a1 = -4.0f;
    renderbliss::real a0 = -3.0f;
    renderbliss::real roots[2] = {0};
    renderbliss::real expectedRoots[2] = {-0.58113883f, 2.5811388f};
    renderbliss::uint nRoots = renderbliss::SolveQuadraticEquation(a2, a1, a0, roots);
    CHECK_EQUAL(nRoots, static_cast<renderbliss::uint>(2));
    renderbliss::real diff1 = fabs(expectedRoots[0]-roots[0]);
    renderbliss::real diff2 = fabs(expectedRoots[1]-roots[1]);
    renderbliss::real diff3 = fabs(expectedRoots[0]-roots[1]);
    renderbliss::real diff4 = fabs(expectedRoots[1]-roots[0]);
    CHECK(((diff1<0.0001f) && (diff2<0.0001f)) || ((diff3<0.0001f) && (diff4<0.0001f)));

    // 9x^2 + 12x + 4 = 0 (one root)
    a2 = 9.0f;
    a1 = 12.0f;
    a0 = 4.0f;
    expectedRoots[0] = -2.0f*renderbliss::Inv3(); // -2/3
    nRoots = renderbliss::SolveQuadraticEquation(a2, a1, a0, roots);
    CHECK_EQUAL(nRoots, static_cast<renderbliss::uint>(1));
    CHECK_CLOSE(expectedRoots[0], roots[0], 0.0001f);

    // 12x + 4 = 0 (one root)
    a2 = 0.0f;
    a1 = 12.0f;
    a0 = 4.0f;
    expectedRoots[0] = -renderbliss::Inv3(); // -1/3
    nRoots = renderbliss::SolveQuadraticEquation(a2, a1, a0, roots);
    CHECK_EQUAL(nRoots, static_cast<renderbliss::uint>(1));
    CHECK_CLOSE(expectedRoots[0], roots[0], 0.0001f);

    // x^2 + 3 = 0 (no root)
    a2 = 1.0f;
    a1 = 0.0f;
    a0 = 3.0f;
    nRoots = renderbliss::SolveQuadraticEquation(a2, a1, a0, roots);
    CHECK_EQUAL(nRoots, static_cast<renderbliss::uint>(0));
}

TEST(CheckSolveLinearSystem2x2)
{
    // 3x - y = 7
    // 2x + 3y = 1
    renderbliss::real A[2][2] =
    {
        {3.0f, -1.0f},
        {2.0f, 3.0f}
    };
    renderbliss::real B[2] = {7.0f, 1.0f};
    renderbliss::real X[2] = {0};
    renderbliss::real expected[2] = {2.0f, -1.0f};
    CHECK(renderbliss::SolveLinearSystem2x2(A, B, X));
    CHECK_ARRAY_CLOSE(expected, X, 2, renderbliss::Epsilon());
}

TEST(CheckSolveLinearSystem3x3)
{
    // x + 2y - z = 2
    // 2x + 2y + 2z = 12
    // x - y + 2z = 5
    renderbliss::real A[3][3] =
    {
        {1.0f, 2.0f, -1.0f},
        {2.0f, 2.0f, 2.0f},
        {1.0f, -1.0f, 2.0f}
    };
    renderbliss::real B[3] = {2.0f, 12.0f, 5.0f};
    renderbliss::real X[3] = {0};
    renderbliss::real expected[3] = {1.0f, 2.0f, 3.0f};
    CHECK(renderbliss::SolveLinearSystem3x3(A, B, X));
    CHECK_ARRAY_CLOSE(expected, X, 3, renderbliss::Epsilon());
}
}
