// ======================================================================== //
// Copyright 2009-2018 Intel Corporation                                    //
//                                                                          //
// Licensed under the Apache License, Version 2.0 (the "License");          //
// you may not use this file except in compliance with the License.         //
// You may obtain a copy of the License at                                  //
//                                                                          //
//     http://www.apache.org/licenses/LICENSE-2.0                           //
//                                                                          //
// Unless required by applicable law or agreed to in writing, software      //
// distributed under the License is distributed on an "AS IS" BASIS,        //
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. //
// See the License for the specific language governing permissions and      //
// limitations under the License.                                           //
// ======================================================================== //

#include "../common/tutorial/tutorial.h"

namespace embree
{
  extern "C" {
    embree::Vec3fa g_dirlight_direction = embree::normalize(embree::Vec3fa(1,-1,1));
    embree::Vec3fa g_dirlight_intensity = embree::Vec3fa(4.0f);
    embree::Vec3fa g_ambient_intensity = embree::Vec3fa(1.0f);
  }
  
  Vec3fa uniformSampleSphere(const float& u, const float& v) 
  {
    const float phi = float(two_pi) * u;
    const float cosTheta = 1.0f - 2.0f * v, sinTheta = 2.0f * sqrt(v * (1.0f - v));
    return Vec3fa(cos(phi) * sinTheta, sin(phi) * sinTheta, cosTheta);
  }

static int p[513] = { 
  151,160,137,91,90,15,131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
  190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
  88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
  77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
  102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
  135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
  5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
  223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
  129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
  251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
  49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
  138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180,
  151,160,137,91,90,15,131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
  190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
  88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
  77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
  102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
  135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
  5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
  223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
  129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
  251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
  49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
  138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180,
  151
};

static float g1[128] = {
  -0.20707f, 0.680971f, -0.293328f, -0.106833f, -0.362614f, 0.772857f, -0.968834f, 0.16818f, -0.681263f, -0.232568f, 0.382009f, -0.882282f, 0.799709f, -0.672908f, -0.681857f, 0.0661294f, 0.208288f, 0.165398f, -0.460058f, -0.219044f, -0.413199f, 0.484755f, -0.402949f, -0.848924f, -0.190035f, 0.714756f, 0.883937f, 0.325661f, 0.692952f, -0.99449f, -0.0752415f, 0.065192f, 0.575753f, -0.468776f, 0.965505f, -0.38643f, 0.20171f, 0.217431f, -0.575122f, 0.77179f, -0.390686f, -0.69628f, -0.324676f, -0.225046f, 0.28722f, 0.507107f, 0.207232f, 0.0632565f, -0.0812794f, 0.304977f, -0.345638f, 0.892741f, -0.26392f, 0.887781f, -0.985143f, 0.0331999f, -0.454458f, -0.951402f, 0.183909f, -0.590073f, 0.755387f, -0.881263f, -0.478315f, -0.394342f, 0.78299f, -0.00360388f, 0.420051f, -0.427172f, 0.729847f, 0.351081f, -0.0830201f, 0.919271f, 0.549351f, -0.246897f, -0.542722f, -0.290932f, -0.399364f, 0.339532f, 0.437933f, 0.131909f, 0.648931f, -0.218776f, 0.637533f, 0.688017f, -0.639064f, 0.886792f, -0.150226f, 0.0413316f, -0.868712f, 0.827016f, 0.765169f, 0.522728f, -0.202155f, 0.376514f, 0.523097f, -0.189982f, -0.749498f, -0.0307322f, -0.555075f, 0.746242f, 0.0576438f, -0.997172f, 0.721028f, -0.962605f, 0.629784f, -0.514232f, -0.370856f, 0.931465f, 0.87112f, 0.618863f, -0.0157817f, -0.559729f, 0.152707f, -0.421942f, -0.357866f, -0.477353f, -0.652024f, -0.996365f, -0.910432f, -0.517651f, -0.169098f, 0.403249f, -0.556309f, 0.00782069f, -0.86594f, -0.213873f, -0.0410469f, -0.563716f
};

static float g2[128*2] = {
0.605609f, 0.538399f, 0.796519f, -0.944204f, 0.908294f, 0.756016f, 0.0977536f, -0.863638f, 0.842196f, -0.744751f, -0.932081f, 0.932392f, -0.588525f, 0.516884f, 0.841188f, -0.978497f, -0.608649f, -0.868011f, 0.992137f, -0.772425f, 0.963049f, -0.0478757f, 0.953878f, 0.889467f, 0.562174f, 0.624644f, -0.356598f, -0.520726f, -0.821833f, 0.99985f, 0.234183f, -0.9791f, -0.971815f, -0.0979374f, -0.108159f, -0.34927f, -0.592124f, -0.775632f, 0.97228f, 0.753819f, 0.941608f, 0.578291f, 0.852108f, -0.760312f, -0.784772f, 0.0223242f, -0.606013f, -0.980319f, 0.252581f, -0.575064f, 0.884701f, 0.943763f, 0.737344f, 0.938496f, 0.0466562f, -0.994566f, 0.989782f, 0.988368f, -0.546155f, 0.279211f, -0.69504f, 0.931229f, 0.99768f, -0.325874f, -0.630157f, -0.999936f, -0.968623f, -0.226805f, -0.750428f, -0.450961f, 0.257868f, 0.968011f, -0.988005f, -0.713965f, 0.991007f, -0.61059f, 0.950437f, -0.483042f, -0.98105f, -0.915356f, -0.892527f, -0.772958f, -0.9081f, 0.55692f, 0.906075f, 0.937419f, 0.454624f, -0.991582f, 0.400857f, 0.855933f, -0.672619f, 0.0713424f, 0.593249f, -0.378286f, -0.997369f, -0.827112f, 0.708222f, -0.995343f, 0.985069f, 0.698711f, -0.180105f, 0.999961f, -0.768451f, 0.993107f, -0.918024f, 0.0446961f, 0.91882f, 0.97691f, -0.393915f, 0.364803f, 0.0495592f, 0.186545f, -0.461553f, -0.242776f, 0.901952f, -0.0710866f, 0.888101f, 0.999935f, 0.277688f, 0.0554235f, 0.506599f, -0.299293f, 0.984394f, -0.999698f, 0.408822f, -0.782639f, 0.128596f, 0.198834f, 0.981707f, 0.864566f, 0.808197f, 0.352335f, 0.970484f, -0.667503f, 0.330243f, 0.208392f, 0.191539f, -0.938943f, 0.895002f, 0.910575f, -0.537691f, -0.98548f, -0.721635f, -0.335382f, -0.424701f, -0.960452f, 0.595047f, 0.783579f, -0.937749f, 0.529096f, -0.997906f, -0.581313f, -0.899828f, -0.88461f, 0.989469f, 0.91872f, -0.850793f, 0.955954f, 0.715768f, -0.736686f, 0.80392f, -0.717276f, -0.788579f, 0.987003f, -0.839648f, 0.885176f, -0.998929f, -0.0376033f, -0.578371f, -0.718771f, 0.906081f, 0.239947f, -0.803563f, -0.00826282f, 0.991011f, -0.0057943f, -0.349232f, 0.65319f, 0.992067f, -0.953535f, 0.893781f, 0.661689f, 0.957253f, -0.425442f, -0.866609f, 0.712892f, -0.807777f, 0.89632f, -0.595147f, -0.0224999f, -0.643786f, 0.545815f, -0.870124f, -0.696306f, -0.99902f, 0.773648f, -0.806008f, -0.931319f, -0.780114f, -0.552154f, -0.933812f, -0.563108f, -0.619909f, 0.966532f, 0.692454f, 0.993284f, 0.338885f, -0.75104f, 0.237272f, -0.713619f, -0.160187f, -0.199242f, -0.371265f, -0.781439f, -0.914125f, -0.944104f, 0.169525f, -0.984403f, 0.976056f, -0.265228f, 0.94232f, 0.993906f, -0.877517f, -0.89618f, 0.611817f, -0.106758f, 0.680403f, 0.163329f, -0.325386f, -0.0687362f, -0.901164f, 0.460314f, 0.999981f, -0.0408026f, 0.850356f, -0.763343f, -0.170806f, -0.102919f, 0.581564f, 0.688634f, 0.284368f, -0.276419f, 0.616641f, -0.929771f, 0.927865f, 0.440373f, 0.153446f, 0.840456f, 0.996966f, 0.867209f, -0.135077f, -0.493238f, -0.577193f, 0.0588088f, 0.715215f, 0.0143633f
};

static float g3[128*4] = {
  -0.582745f, 0.443494f, -0.680971f, 0.0f, -0.601153f, 0.791961f, 0.106833f, 0.0f, -0.265466f, 0.576385f, -0.772857f, 0.0f, 0.981035f, 0.0963612f, -0.16818f, 0.0f, 0.524388f, 0.819103f, 0.232568f, 0.0f, -0.170518f, -0.43875f, 0.882282f, 0.0f, 0.598053f, -0.435348f, 0.672908f, 0.0f, 0.53956f, 0.839346f, -0.0661294f, 0.0f, -0.782511f, -0.600267f, -0.165398f, 0.0f, -0.122114f, 0.968043f, 0.219044f, 0.0f, -0.235567f, 0.842331f, -0.484755f, 0.0f, -0.158657f, 0.504139f, 0.848924f, 0.0f, -0.578396f, 0.39317f, -0.714756f, 0.0f, 0.883328f, -0.337159f, -0.325661f, 0.0f, 0.0597264f, -0.0861552f, 0.99449f, 0.0f, -0.970124f, 0.233685f, -0.0651921f, 0.0f, 0.208238f, -0.858421f, 0.468776f, 0.0f, 0.916908f, -0.0997567f, 0.38643f, 0.0f, -0.786568f, -0.577957f, -0.217431f, 0.0f, 0.14868f, 0.618251f, -0.77179f, 0.0f, -0.24168f, 0.675858f, 0.69628f, 0.0f, -0.50994f, 0.83025f, 0.225046f, 0.0f, -0.534183f, -0.676382f, -0.507107f, 0.0f, -0.793861f, -0.6048f, -0.0632565f, 0.0f, -0.92148f, 0.240548f, -0.304977f, 0.0f, -0.210037f, 0.39862f, -0.892741f, 0.0f, -0.310918f, 0.339375f, -0.887781f, 0.0f, 0.99836f, 0.0466305f, -0.0331999f, 0.0f, -0.0439099f, 0.304806f, 0.951402f, 0.0f, -0.676304f, -0.440938f, 0.590073f, 0.0f, 0.339805f, -0.328495f, 0.881263f, 0.0f, -0.0625568f, 0.916832f, 0.394342f, 0.0f, 0.776463f, -0.630153f, 0.00360388f, 0.0f, -0.224717f, -0.8758f, 0.427172f, 0.0f, 0.618879f, -0.70266f, -0.351081f, 0.0f, -0.380313f, 0.101503f, -0.919271f, 0.0f, 0.149639f, -0.957418f, 0.246897f, 0.0f, 0.128024f, 0.948139f, 0.290932f, 0.0f, -0.292448f, 0.893976f, -0.339532f, 0.0f, -0.192062f, -0.972477f, -0.131909f, 0.0f, 0.44007f, -0.870905f, 0.218776f, 0.0f, 0.303887f, -0.659003f, -0.688017f, 0.0f, 0.195552f, 0.41876f, -0.886792f, 0.0f, -0.889922f, 0.454236f, -0.0413315f, 0.0f, 0.515034f, 0.225353f, -0.827016f, 0.0f, 0.63084f, -0.573408f, -0.522728f, 0.0f, -0.745779f, 0.549592f, -0.376514f, 0.0f, 0.0711763f, -0.979204f, 0.189982f, 0.0f, 0.705657f, 0.707887f, 0.0307322f, 0.0f, 0.114603f, 0.655735f, -0.746242f, 0.0f, -0.0739232f, -0.0135353f, 0.997172f, 0.0f, 0.173356f, -0.20818f, 0.962605f, 0.0f, 0.34008f, -0.787344f, 0.514232f, 0.0f, -0.143596f, 0.334295f, -0.931465f, 0.0f, 0.721989f, -0.30942f, -0.618863f, 0.0f, -0.827657f, 0.0410685f, 0.559729f, 0.0f, -0.804277f, -0.418454f, 0.421942f, 0.0f, -0.379459f, 0.792556f, 0.477353f, 0.0f, 0.0391537f, 0.0756503f, 0.996365f, 0.0f, 0.821943f, 0.237588f, 0.517651f, 0.0f, -0.788974f, 0.463584f, -0.403249f, 0.0f, 0.175972f, 0.984364f, -0.00782073f, 0.0f, 0.891497f, 0.399363f, 0.213873f, 0.0f, -0.819111f, 0.106216f, 0.563716f, 0.0f, 0.105511f, 0.544028f, -0.832406f, 0.0f, -0.464551f, 0.63753f, 0.614612f, 0.0f, 0.232387f, 0.935154f, -0.267363f, 0.0f, 0.777619f, 0.272068f, -0.566823f, 0.0f, 0.975331f, 0.190338f, 0.111807f, 0.0f, 0.224313f, 0.450072f, -0.86436f, 0.0f, 0.841897f, -0.536898f, 0.0543103f, 0.0f, 0.637123f, -0.664145f, -0.391135f, 0.0f, 0.901675f, -0.422984f, 0.0898189f, 0.0f, -0.496241f, 0.367413f, -0.786608f, 0.0f, -0.255468f, -0.689763f, -0.677469f, 0.0f, -0.0616459f, -0.951141f, -0.302539f, 0.0f, -0.431011f, -0.889035f, -0.154425f, 0.0f, -0.0711688f, 0.486502f, -0.870776f, 0.0f, -0.223359f, -0.36162f, 0.905175f, 0.0f, -0.678546f, 0.695482f, -0.23639f, 0.0f, 0.576553f, 0.77934f, 0.245389f, 0.0f, -0.194568f, -0.24951f, 0.948624f, 0.0f, 0.28962f, -0.447736f, 0.845962f, 0.0f, -0.0403821f, -0.871893f, 0.488028f, 0.0f, 0.790972f, -0.560788f, 0.244705f, 0.0f, -0.34553f, 0.739953f, 0.57713f, 0.0f, -0.516376f, -0.697122f, 0.49737f, 0.0f, 0.115998f, 0.859293f, 0.498156f, 0.0f, 0.643831f, -0.239955f, 0.72657f, 0.0f, -0.125114f, 0.987348f, -0.0974144f, 0.0f, -0.306452f, 0.610699f, -0.73016f, 0.0f, -0.269845f, 0.893027f, -0.360119f, 0.0f, 0.328563f, -0.570628f, -0.752615f, 0.0f, -0.306918f, -0.42057f, 0.853769f, 0.0f, 0.699245f, -0.51785f, 0.492837f, 0.0f, -0.558362f, -0.469763f, -0.68378f, 0.0f, 0.476563f, -0.841398f, 0.254826f, 0.0f, 0.0276172f, -0.623206f, 0.78157f, 0.0f, 0.587723f, -0.800313f, -0.118659f, 0.0f, 0.594035f, -0.740708f, 0.313806f, 0.0f, -0.340185f, -0.887929f, 0.309605f, 0.0f, 0.312245f, -0.246681f, -0.917416f, 0.0f, 0.194206f, 0.186398f, -0.963089f, 0.0f, 0.915704f, 0.329835f, -0.229553f, 0.0f, 0.94133f, 0.229917f, 0.247055f, 0.0f, -0.888253f, -0.144148f, 0.436152f, 0.0f, -0.906917f, -0.362625f, -0.214486f, 0.0f, 0.403108f, -0.908884f, 0.10693f, 0.0f, 0.983963f, 0.169256f, 0.056292f, 0.0f, -0.197949f, 0.888236f, 0.414553f, 0.0f, 0.0879741f, 0.247673f, 0.964841f, 0.0f, 0.474384f, -0.868071f, -0.146331f, 0.0f, 0.699884f, 0.541342f, -0.465953f, 0.0f, 0.610965f, 0.567249f, 0.552223f, 0.0f, 0.830508f, -0.285788f, -0.478103f, 0.0f, 0.328573f, -0.683076f, -0.652263f, 0.0f, -0.00537775f, 0.873381f, 0.487009f, 0.0f, -0.51289f, 0.828835f, 0.223557f, 0.0f, -0.871168f, -0.15102f, 0.467182f, 0.0f, -0.545561f, 0.390016f, -0.741789f, 0.0f, 0.874063f, 0.259258f, 0.410852f, 0.0f, -0.781555f, 0.612184f, -0.120005f, 0.0f, -0.284928f, 0.708938f, -0.645154f, 0.0f, -0.568809f, 0.0883274f, 0.817713f, 0.0f, -0.0429388f, 0.549957f, -0.834088f, 0.0f, 0.933296f, -0.127233f, 0.335813f, 0.0f, 0.698149f, -0.493464f, 0.51873f, 0.0f, -0.603413f, 0.617495f, -0.504572f, 0 
};

__forceinline float fade(float t) {
  return (t * t * t) * (t * (t * 6 - 15) + 10); 
}

/*__forceinline float fade(float t) {
  return t * t * (3 - t * 2); 
  }*/

__forceinline float mylerp(float t, float a, float b) { 
  return a + t * (b - a); 
}

__forceinline float grad(int hash, float x) {
  return x*g1[hash&127];
}

__forceinline float grad(int hash, float x, float y) {
  int h = hash&127;
  return x*g2[2*h+0]+y*g2[2*h+1];
}

__forceinline float grad(int hash, float x, float y, float z) {
  int h = hash&127;
  return x*g3[4*h+0]+y*g3[4*h+1]+z*g3[4*h+2];
}

float Noise(float x) 
{
  float fx = floorf(x);
  int X = (int)fx & 255;
  x -= fx;
  float u = fade(x);
  float g00 = grad(p[X  ],x  );
  float g10 = grad(p[X+1],x-1);
  return mylerp(u,g00,g10);
}

float Noise(float x, float y) 
{
  float fx = floorf(x);
  float fy = floorf(y);
  
  int X = (int)fx & 255;
  int Y = (int)fy & 255;
  
  x -= fx;
  y -= fy;
  
  float u = fade(x);
  float v = fade(y);
  
  int p00  = p[X  ]+Y;
  int p10  = p[X+1]+Y;
  int p01  = p[X  ]+Y+1;
  int p11  = p[X+1]+Y+1;
  
  float g00 = grad(p[p00],x  ,y  );
  float g10 = grad(p[p10],x-1,y  );
  float g01 = grad(p[p01],x  ,y-1);
  float g11 = grad(p[p11],x-1,y-1);
  
  return mylerp(v,mylerp(u,g00,g10),mylerp(u,g01,g11));
}

float Noise(float x, float y, float z) 
{
  float fx = floorf(x);
  float fy = floorf(y);
  float fz = floorf(z);
  
  int X = (int)fx & 255;
  int Y = (int)fy & 255;
  int Z = (int)fz & 255;
  
  x -= fx;
  y -= fy;
  z -= fz;
  
  float u = fade(x);
  float v = fade(y);
  float w = fade(z);
  
  int p00  = p[X]+Y;
  int p000 = p[p00]+Z;
  int p010 = p[p00+1]+Z;
  int p001 = p000+1; 
  int p011 = p010+1;
  int p10  = p[X+1]+Y;
  int p100 = p[p10]+Z;
  int p110 = p[p10+1]+Z;
  int p101 = p100+1;
  int p111 = p110+1;
  
  float g000 = grad(p[p000],x  ,y  ,z  );
  float g100 = grad(p[p100],x-1,y  ,z  );
  float g010 = grad(p[p010],x  ,y-1,z  );
  float g110 = grad(p[p110],x-1,y-1,z  );
  float g001 = grad(p[p001],x  ,y  ,z-1);
  float g101 = grad(p[p101],x-1,y  ,z-1);
  float g011 = grad(p[p011],x  ,y-1,z-1);
  float g111 = grad(p[p111],x-1,y-1,z-1);
  
  return mylerp(w,
              mylerp(v,mylerp(u,g000,g100),mylerp(u,g010,g110)),
              mylerp(v,mylerp(u,g001,g101),mylerp(u,g011,g111)));
}

  Vec3fa Noise3D(const Vec3fa& p)
  {
    float x = Noise(4.0f*p.x);
    float y = Noise(4.0f*p.y);
    float z = Noise(4.0f*p.z);
    return p+0.2f*Vec3fa(x,y,z);
  }

  void addHairySphere (Ref<SceneGraph::GroupNode>& scene, const Vec3fa& p, float r)
  {
    Ref<SceneGraph::MaterialNode> material = new HairMaterial(Vec3fa(1.0f,1.0f,0.0f),Vec3fa(0.8f),2.0f,2.0f);
    
    {
      const size_t numPhi   = 20;
      const size_t numTheta = 2*numPhi;
      
      avector<Vec3fa> positions;
      avector<Vec3fa> normals;
      std::vector<Vec2f> texcoords;
      std::vector<SceneGraph::TriangleMeshNode::Triangle> triangles;

      const float rcpNumTheta = rcp((float)numTheta);
      const float rcpNumPhi   = rcp((float)numPhi);
      for (unsigned int phi=0; phi<=numPhi; phi++)
      {
        for (unsigned int theta=0; theta<numTheta; theta++)
        {
          const float phif   = phi*float(pi)*rcpNumPhi;
          const float thetaf = theta*2.0f*float(pi)*rcpNumTheta;
          const Vec3fa dp(sin(phif)*sin(thetaf),cos(phif),sin(phif)*cos(thetaf));
          positions.push_back(p+r*dp);
          normals.push_back(dp);
        }
        if (phi == 0) continue;
        
        for (unsigned int theta=1; theta<=numTheta; theta++) 
        {
          unsigned int p00 = (phi-1)*numTheta+theta-1;
          unsigned int p01 = (phi-1)*numTheta+theta%numTheta;
          unsigned int p10 = phi*numTheta+theta-1;
          unsigned int p11 = phi*numTheta+theta%numTheta;
          
          if (phi > 1)
            triangles.push_back(SceneGraph::TriangleMeshNode::Triangle(p10,p00,p01));
          
          if (phi < numPhi) 
            triangles.push_back(SceneGraph::TriangleMeshNode::Triangle(p11,p10,p01));
        }
      }
      
      scene->add(new SceneGraph::TriangleMeshNode(positions,normals,texcoords,triangles,material));
    }

    {
      const float thickness = 0.001f*r;
      avector<Vec3fa> positions; 
      std::vector<SceneGraph::HairSetNode::Hair> hairs;
      
      unsigned int s = 0;
      for (size_t iy=0; iy<300; iy++)
      {
        for (size_t ix=0; ix<300; ix++)
        {
          float fx = (float(ix)+2.0f*g2[s])/300.0f; s=(s+1)%255;
          float fy = (float(iy)+2.0f*g2[s])/300.0f; s=(s+1)%255;
          Vec3fa dp = uniformSampleSphere(fx,fy);
          
          Vec3fa l0 = p + r*       (dp + 0.00f*dp); l0.w = thickness;
          Vec3fa l1 = p + r*       (dp + 0.25f*dp); l1.w = thickness;
          Vec3fa l2 = p + r*Noise3D(dp + 0.50f*dp); l2.w = thickness;
          Vec3fa l3 = p + r*Noise3D(dp + 0.75f*dp); l3.w = thickness;
          
          const size_t v_index = positions.size();
          positions.push_back(l0);
          positions.push_back(l1);
          positions.push_back(l2);
          positions.push_back(l3);
          hairs.push_back(SceneGraph::HairSetNode::Hair(unsigned(v_index),0) );
        }
      }
      scene->add(new SceneGraph::HairSetNode(positions,hairs,material,RTC_GEOMETRY_TYPE_FLAT_BEZIER_CURVE));
    }
  }

  void addGroundPlane (Ref<SceneGraph::GroupNode>& scene, const Vec3fa& p00, const Vec3fa& p01, const Vec3fa& p10, const Vec3fa& p11)
  {
     avector<Vec3fa> positions;
     avector<Vec3fa> normals;
     std::vector<Vec2f> texcoords;
     std::vector<SceneGraph::TriangleMeshNode::Triangle> triangles;

     Ref<SceneGraph::MaterialNode> material = new OBJMaterial;
   
     positions.push_back(p00);
     positions.push_back(p01);
     positions.push_back(p10);
     positions.push_back(p11);

     triangles.push_back(SceneGraph::TriangleMeshNode::Triangle(0,1,2));
     triangles.push_back(SceneGraph::TriangleMeshNode::Triangle(2,1,3));
     
     scene->add(new SceneGraph::TriangleMeshNode(positions,normals,texcoords,triangles,material));
  }

  struct Tutorial : public TutorialApplication 
  {
    Tutorial()
      : TutorialApplication("hair_geometry",FEATURE_RTCORE) 
    {
      registerOption("ambientlight", [] (Ref<ParseStream> cin, const FileName& path) {
          g_ambient_intensity = cin->getVec3fa();
        }, "--ambientlight r g b: adds an ambient light with intensity rgb");
      registerOptionAlias("ambientlight","ambient");

      registerOption("directionallight", [] (Ref<ParseStream> cin, const FileName& path) {
          g_dirlight_direction = normalize(cin->getVec3fa());
          g_dirlight_intensity = cin->getVec3fa();
        }, "--directionallight x y z r g b: adds a directional light with direction xyz and intensity rgb");
      registerOptionAlias("directionallight","dirlight");
      
      /* set default camera */
      camera.from = Vec3fa(-3,3,3);
      camera.to = Vec3fa(0,1,0);
      camera.up = Vec3fa(0,1,0);
    }

    void postParseCommandLine() 
    {
      Ref<SceneGraph::GroupNode> group = new SceneGraph::GroupNode;
      addHairySphere(group,Vec3fa(0,1.5f,0),1.5f);
      addGroundPlane(group,Vec3fa(-10,0,-10),Vec3fa(-10,0,+10),Vec3fa(+10,0,-10),Vec3fa(+10,0,+10));

      /* convert model */
      obj_scene.add(SceneGraph::flatten(group,SceneGraph::INSTANCING_NONE)); 
      
      /* send model */
      set_scene(&obj_scene);
    }

    TutorialScene obj_scene;
  };

}

int main(int argc, char** argv) {
  return embree::Tutorial().main(argc,argv);
}
