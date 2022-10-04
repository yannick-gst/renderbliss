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

#include <vector>
#include <log++/Log++.h>
#include <log++/FileDestination.h>
#include "renderbliss/Scene.h"
#include "renderbliss/Camera/ThinLensCamera.h"
#include "renderbliss/Camera/Film/ImageFilm.h"
#include "renderbliss/Colour/Spectrum.h"
#include "renderbliss/Colour/Data/CornellBox.h"
#include "renderbliss/ImageIO/ImageIO.h"
#include "renderbliss/Integrators/PathIntegrator.h"
#include "renderbliss/Integrators/PhotonIntegrator.h"
#include "renderbliss/Integrators/WhittedIntegrator.h"
#include "renderbliss/Lights/Luminaire.h"
#include "renderbliss/Materials/LambertianMaterial.h"
#include "renderbliss/Math/Geometry/Vector2.h"
#include "renderbliss/Math/Geometry/Vector3.h"
#include "renderbliss/Math/Sampling/Filters/TriangleFilter.h"
#include "renderbliss/Primitives/MeshPrimitive.h"
#include "renderbliss/Rendering/Renderer.h"
#include "renderbliss/Textures/ConstantTexture.h"
#include "renderbliss/Utils/JobScheduler.h"
#include "renderbliss/Utils/JobScheduler.h"
#include "renderbliss/Utils/PropertyMap.h"
#include "renderbliss/Utils/StatsTracker.h"

int main()
{
    logpp::LogDestinationPtr fileDest(new logpp::LogFileDestination("renderbliss.log", true));
    logpp::GlobalLogger().AddDestination(fileDest);
    logpp::GlobalLogger().AddDestination(logpp::LogDestination::StdLog());

    // This program uses RenderBliss to render the Cornell Box
    // http://www.graphics.cornell.edu/online/box/

    using namespace renderbliss;

    PropertyMap gpmProps;
    gpmProps.Set<real>("gather_radius", 100.0f);
    gpmProps.Set<uint32>("photons_to_store", 50000);
    gpmProps.Set<uint32>("photons_to_gather", 50);
    gpmProps.Set<uint32>("precomputed_irradiance_spacing", 4);
    gpmProps.Set<float>("precomputed_irradiance_normal_threshold", 0.9f);
    PropertyMap props;
    props.Set<uint32>("final_gathering_samples", 4);
    props.Set<uint32>("path_depth", 4);
    props.Set<uint32>("photon_depth", 4);
    props.Set<uint32>("shadow_rays", 9);
    props.Set<PropertyMap>("global_photon_map", gpmProps);
    std::auto_ptr<Scene> scene(new Scene(props));

    // Textures
    boost::shared_ptr<const ITexture> light(new ConstantTexture(CornellLightReflectance()));
    boost::shared_ptr<const ITexture> red(new ConstantTexture(CornellRed()));
    boost::shared_ptr<const ITexture> green(new ConstantTexture(CornellGreen()));
    boost::shared_ptr<const ITexture> white(new ConstantTexture(CornellWhite()));

    // Materials
    boost::shared_ptr<const IMaterial> redMaterial(new LambertianMaterial(red));
    boost::shared_ptr<const IMaterial> greenMaterial(new LambertianMaterial(green));
    boost::shared_ptr<const IMaterial> lightMaterial(new LambertianMaterial(light));
    boost::shared_ptr<const IMaterial> whiteMaterial(new LambertianMaterial(white));

    std::vector<Vector2> uvs;
    std::vector<Vector3> normals;
    std::vector<Vector3> vertices;
    std::vector<size_t> vertexIndices;
    vertexIndices.push_back(0);
    vertexIndices.push_back(1);
    vertexIndices.push_back(2);
    vertexIndices.push_back(0);
    vertexIndices.push_back(2);
    vertexIndices.push_back(3);

    // Light source
    {
        vertices.clear();
        vertices.push_back(Vector3(343.0f, 548.799f, 332.0f));
        vertices.push_back(Vector3(213.0f, 548.799f, 332.0f));
        vertices.push_back(Vector3(213.0f, 548.799f, 227.0f));
        vertices.push_back(Vector3(343.0f, 548.799f, 227.0f));
        MeshPrimitive* lightMesh = new MeshPrimitive(2, vertexIndices, vertices, normals, uvs, lightMaterial);
        TrianglePrimitiveList triangles;
        lightMesh->Refine(triangles);
        const Luminaire* lum = new Luminaire(triangles, CornellLightExitance());
        lightMesh->SetEmissionProfile(lum);
        scene->Lights().push_back(boost::shared_ptr<const ILight>(lum));
        scene->Meshes().push_back(boost::shared_ptr<const MeshPrimitive>(lightMesh));
    }
    // Floor
    {
        vertices.clear();
        vertices.push_back(Vector3(552.8f, 0.0f, 0.0f));
        vertices.push_back(Vector3(0.0f  , 0.0f, 0.0f));
        vertices.push_back(Vector3(0.0f  , 0.0f, 559.2f));
        vertices.push_back(Vector3(549.6f, 0.0f, 559.2f));
        MeshConstPtr floorMesh(new MeshPrimitive(2, vertexIndices, vertices, normals, uvs, whiteMaterial));
        scene->Meshes().push_back(floorMesh);
    }
    // Ceiling
    {
        vertices.clear();
        vertices.push_back(Vector3(556.0f, 548.8f, 559.2f));
        vertices.push_back(Vector3(  0.0f, 548.8f, 559.2f));
        vertices.push_back(Vector3(  0.0f, 548.8f,   0.0f));
        vertices.push_back(Vector3(556.0f, 548.8f,   0.0f));
        MeshConstPtr ceilingMesh(new MeshPrimitive(2, vertexIndices, vertices, normals, uvs, whiteMaterial));
        scene->Meshes().push_back(ceilingMesh);
    }
    // Back wall
    {
        vertices.clear();
        vertices.push_back(Vector3(549.6f,   0.0f, 559.2f));
        vertices.push_back(Vector3(  0.0f,   0.0f, 559.2f));
        vertices.push_back(Vector3(  0.0f, 548.8f, 559.2f));
        vertices.push_back(Vector3(556.0f, 548.8f, 559.2f));
        MeshConstPtr backMesh(new MeshPrimitive(2, vertexIndices, vertices, normals, uvs, whiteMaterial));
        scene->Meshes().push_back(backMesh);
    }
    // Left wall
    {
        vertices.clear();
        vertices.push_back(Vector3(552.8f,   0.0f,   0.0f));
        vertices.push_back(Vector3(549.6f,   0.0f, 559.2f));
        vertices.push_back(Vector3(556.0f, 548.8f, 559.2f));
        vertices.push_back(Vector3(556.0f, 548.8f,   0.0f));
        MeshConstPtr leftMesh(new MeshPrimitive(2, vertexIndices, vertices, normals, uvs, redMaterial));
        scene->Meshes().push_back(leftMesh);
    }
    // Right wall
    {
        vertices.clear();
        vertices.push_back(Vector3(0.0f,   0.0f, 559.2f));
        vertices.push_back(Vector3(0.0f,   0.0f,   0.0f));
        vertices.push_back(Vector3(0.0f, 548.8f,   0.0f));
        vertices.push_back(Vector3(0.0f, 548.8f, 559.2f));
        MeshConstPtr rightMesh(new MeshPrimitive(2, vertexIndices, vertices, normals, uvs, greenMaterial));
        scene->Meshes().push_back(rightMesh);
    }
    // Short block top
    {
        vertices.clear();
        vertices.push_back(Vector3(130.0f, 165.0f,  65.0f));
        vertices.push_back(Vector3( 82.0f, 165.0f, 225.0f));
        vertices.push_back(Vector3(240.0f, 165.0f, 272.0f));
        vertices.push_back(Vector3(290.0f, 165.0f, 114.0f));
        MeshConstPtr shortTopMesh(new MeshPrimitive(2, vertexIndices, vertices, normals, uvs, whiteMaterial));
        scene->Meshes().push_back(shortTopMesh);
    }
    // Short block left
    {
        vertices.clear();
        vertices.push_back(Vector3(290.0f,   0.0f, 114.0f));
        vertices.push_back(Vector3(290.0f, 165.0f, 114.0f));
        vertices.push_back(Vector3(240.0f, 165.0f, 272.0f));
        vertices.push_back(Vector3(240.0f,   0.0f, 272.0f));
        MeshConstPtr shortLeftMesh(new MeshPrimitive(2, vertexIndices, vertices, normals, uvs, whiteMaterial));
        scene->Meshes().push_back(shortLeftMesh);
    }
    // Short block front
    {
        vertices.clear();
        vertices.push_back(Vector3(130.0f,   0.0f,  65.0f));
        vertices.push_back(Vector3(130.0f, 165.0f,  65.0f));
        vertices.push_back(Vector3(290.0f, 165.0f, 114.0f));
        vertices.push_back(Vector3(290.0f,   0.0f, 114.0f));
        MeshConstPtr shortFrontMesh(new MeshPrimitive(2, vertexIndices, vertices, normals, uvs, whiteMaterial));
        scene->Meshes().push_back(shortFrontMesh);
    }
    // Short block right
    {
        vertices.clear();
        vertices.push_back(Vector3( 82.0f,   0.0f, 225.0f));
        vertices.push_back(Vector3( 82.0f, 165.0f, 225.0f));
        vertices.push_back(Vector3(130.0f, 165.0f,  65.0f));
        vertices.push_back(Vector3(130.0f,   0.0f,  65.0f));
        MeshConstPtr shortRightMesh(new MeshPrimitive(2, vertexIndices, vertices, normals, uvs, whiteMaterial));
        scene->Meshes().push_back(shortRightMesh);
    }
    // Short block back
    {
        vertices.clear();
        vertices.push_back(Vector3(240.0f,   0.0f, 272.0f));
        vertices.push_back(Vector3(240.0f, 165.0f, 272.0f));
        vertices.push_back(Vector3( 82.0f, 165.0f, 225.0f));
        vertices.push_back(Vector3( 82.0f,   0.0f, 225.0f));
        MeshConstPtr shortBackMesh(new MeshPrimitive(2, vertexIndices, vertices, normals, uvs, whiteMaterial));
        scene->Meshes().push_back(shortBackMesh);
    }
    // Tall block top
    {
        vertices.clear();
        vertices.push_back(Vector3(423.0f, 330.0f, 247.0f));
        vertices.push_back(Vector3(265.0f, 330.0f, 296.0f));
        vertices.push_back(Vector3(314.0f, 330.0f, 456.0f));
        vertices.push_back(Vector3(472.0f, 330.0f, 406.0f));
        MeshConstPtr tallTopMesh(new MeshPrimitive(2, vertexIndices, vertices, normals, uvs, whiteMaterial));
        scene->Meshes().push_back(tallTopMesh);
    }
    // Tall block left
    {
        vertices.clear();
        vertices.push_back(Vector3(423.0f,   0.0f, 247.0f));
        vertices.push_back(Vector3(423.0f, 330.0f, 247.0f));
        vertices.push_back(Vector3(472.0f, 330.0f, 406.0f));
        vertices.push_back(Vector3(472.0f,   0.0f, 406.0f));
        MeshConstPtr tallLeftMesh(new MeshPrimitive(2, vertexIndices, vertices, normals, uvs, whiteMaterial));
        scene->Meshes().push_back(tallLeftMesh);
    }
    // Tall block back
    {
        vertices.clear();
        vertices.push_back(Vector3(472.0f,   0.0f, 406.0f));
        vertices.push_back(Vector3(472.0f, 330.0f, 406.0f));
        vertices.push_back(Vector3(314.0f, 330.0f, 456.0f));
        vertices.push_back(Vector3(314.0f,   0.0f, 456.0f));
        MeshConstPtr tallBackMesh(new MeshPrimitive(2, vertexIndices, vertices, normals, uvs, whiteMaterial));
        scene->Meshes().push_back(tallBackMesh);
    }
    // Tall block right
    {
        vertices.clear();
        vertices.push_back(Vector3(314.0f,   0.0f, 456.0f));
        vertices.push_back(Vector3(314.0f, 330.0f, 456.0f));
        vertices.push_back(Vector3(265.0f, 330.0f, 296.0f));
        vertices.push_back(Vector3(265.0f,   0.0f, 296.0f));
        MeshConstPtr tallRightMesh(new MeshPrimitive(2, vertexIndices, vertices, normals, uvs, whiteMaterial));
        scene->Meshes().push_back(tallRightMesh);
    }
    // Tall block front
    {
        vertices.clear();
        vertices.push_back(Vector3(265.0f,   0.0f, 296.0f));
        vertices.push_back(Vector3(265.0f, 330.0f, 296.0f));
        vertices.push_back(Vector3(423.0f, 330.0f, 247.0f));
        vertices.push_back(Vector3(423.0f,   0.0f, 247.0f));
        MeshConstPtr tallFrontMesh(new MeshPrimitive(2, vertexIndices, vertices, normals, uvs, whiteMaterial));
        scene->Meshes().push_back(tallFrontMesh);
    }

    scene->PreProcess();
    boost::shared_ptr<const Scene> scn(scene.release());

    boost::shared_ptr<IFilter> filter(new TriangleFilter(1.0f, 1.0f));
    boost::shared_ptr<IToneMapper> dummyToneMapper;

    JobScheduler jobScheduler;

    //{
    //    boost::shared_ptr<IFilm> film(new ImageFilm(512, 512, filter, dummyToneMapper));
    //    boost::shared_ptr<const ICamera> camera(new ThinLensCamera(film, 4, Vector3(278.0f, 273.0f, -800.0f), Vector3(278.0f, 273.0f, 1.0f), Vector3::unitY, 37.0f, 800.0f, 0.025f));
    //    StatsTracker stats;
    //    boost::shared_ptr<SurfaceIntegrator> whitted(new WhittedIntegrator(props, stats));
    //    Renderer renderer(props, camera, scn, whitted, jobScheduler, stats);
    //    renderer.Render();
    //    RGBPixelList pixels;
    //    camera->Film()->StorePixels(pixels);
    //    SavePNG("cornell-whitted.png", pixels, film->XResolution(), film->YResolution());
    //    stats.Log();
    //}

    //{
    //    boost::shared_ptr<IFilm> film(new ImageFilm(512, 512, filter, dummyToneMapper));
    //    boost::shared_ptr<const ICamera> camera(new ThinLensCamera(film, 4, Vector3(278.0f, 273.0f, -800.0f), Vector3(278.0f, 273.0f, 1.0f), Vector3::unitY, 37.0f, 800.0f, 0.025f));
    //    StatsTracker stats;
    //    boost::shared_ptr<SurfaceIntegrator> photon(new PathIntegrator(props, stats));
    //    Renderer renderer(props, camera, scn, photon, jobScheduler, stats);
    //    renderer.Render();
    //    RGBPixelList pixels;
    //    camera->Film()->StorePixels(pixels);
    //    SavePNG("cornell-path.png", pixels, film->XResolution(), film->YResolution());
    //    stats.Log();
    //}

    {
        boost::shared_ptr<IFilm> film(new ImageFilm(512, 512, filter, dummyToneMapper));
        boost::shared_ptr<const ICamera> camera(new ThinLensCamera(film, 16, Vector3(278.0f, 273.0f, -800.0f), Vector3(278.0f, 273.0f, 1.0f), Vector3::unitY, 37.0f, 800.0f, 0.025f));
        StatsTracker stats;
        boost::shared_ptr<SurfaceIntegrator> photon(new PhotonIntegrator(props, stats));
        Renderer renderer(props, camera, scn, photon, jobScheduler, stats);
        renderer.Render();
        RGBPixelList pixels;
        camera->Film()->StorePixels(pixels);
        SavePNG("cornell-photon-map.png", pixels, film->XResolution(), film->YResolution());
        stats.Log();
    }

    return 0;
}
