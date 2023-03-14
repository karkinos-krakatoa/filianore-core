#ifndef _METALLURGY_H
#define _METALLURGY_H

#include "principalspectrum.h"
#include <map>
#include <string>
#include <utility>
#include <vector>

namespace filianore {
inline PrincipalSpectrum GoldEta, GoldK;
const int Gold_nSamples = 56;
const float Gold_Lambda[Gold_nSamples] = {
    298.757050f,
    302.400421f,
    306.133759f,
    309.960449f,
    313.884003f,
    317.908142f,
    322.036835f,
    326.274139f,
    330.624481f,
    335.092377f,
    339.682678f,
    344.400482f,
    349.251221f,
    354.240509f,
    359.374420f,
    364.659332f,
    370.102020f,
    375.709625f,
    381.489777f,
    387.450562f,
    393.600555f,
    399.948975f,
    406.505493f,
    413.280579f,
    420.285339f,
    427.531647f,
    435.032196f,
    442.800629f,
    450.851562f,
    459.200653f,
    467.864838f,
    476.862213f,
    486.212463f,
    495.936707f,
    506.057861f,
    516.600769f,
    527.592224f,
    539.061646f,
    551.040771f,
    563.564453f,
    576.670593f,
    590.400818f,
    604.800842f,
    619.920898f,
    635.816284f,
    652.548279f,
    670.184753f,
    688.800964f,
    708.481018f,
    729.318665f,
    751.419250f,
    774.901123f,
    799.897949f,
    826.561157f,
    855.063293f,
    885.601257f};
const float Gold_Eta[Gold_nSamples] = {
    1.795000f,
    1.812000f,
    1.822625f,
    1.830000f,
    1.837125f,
    1.840000f,
    1.834250f,
    1.824000f,
    1.812000f,
    1.798000f,
    1.782000f,
    1.766000f,
    1.752500f,
    1.740000f,
    1.727625f,
    1.716000f,
    1.705875f,
    1.696000f,
    1.684750f,
    1.674000f,
    1.666000f,
    1.658000f,
    1.647250f,
    1.636000f,
    1.628000f,
    1.616000f,
    1.596250f,
    1.562000f,
    1.502125f,
    1.426000f,
    1.345875f,
    1.242000f,
    1.086750f,
    0.916000f,
    0.754500f,
    0.608000f,
    0.491750f,
    0.402000f,
    0.345500f,
    0.306000f,
    0.267625f,
    0.236000f,
    0.212375f,
    0.194000f,
    0.177750f,
    0.166000f,
    0.161000f,
    0.160000f,
    0.160875f,
    0.164000f,
    0.169500f,
    0.176000f,
    0.181375f,
    0.188000f,
    0.198125f,
    0.210000f};
const float Gold_K[Gold_nSamples] = {
    1.920375f,
    1.920000f,
    1.918875f,
    1.916000f,
    1.911375f,
    1.904000f,
    1.891375f,
    1.878000f,
    1.868250f,
    1.860000f,
    1.851750f,
    1.846000f,
    1.845250f,
    1.848000f,
    1.852375f,
    1.862000f,
    1.883000f,
    1.906000f,
    1.922500f,
    1.936000f,
    1.947750f,
    1.956000f,
    1.959375f,
    1.958000f,
    1.951375f,
    1.940000f,
    1.924500f,
    1.904000f,
    1.875875f,
    1.846000f,
    1.814625f,
    1.796000f,
    1.797375f,
    1.840000f,
    1.956500f,
    2.120000f,
    2.326250f,
    2.540000f,
    2.730625f,
    2.880000f,
    2.940625f,
    2.970000f,
    3.015000f,
    3.060000f,
    3.070000f,
    3.150000f,
    3.445812f,
    3.800000f,
    4.087687f,
    4.357000f,
    4.610188f,
    4.860000f,
    5.125813f,
    5.390000f,
    5.631250f,
    5.880000f};

inline PrincipalSpectrum CopperEta, CopperK;
const int Copper_nSamples = 56;
const float Copper_Lambda[Copper_nSamples] = {
    298.757050f,
    302.400421f,
    306.133759f,
    309.960449f,
    313.884003f,
    317.908142f,
    322.036835f,
    326.274139f,
    330.624481f,
    335.092377f,
    339.682678f,
    344.400482f,
    349.251221f,
    354.240509f,
    359.374420f,
    364.659332f,
    370.102020f,
    375.709625f,
    381.489777f,
    387.450562f,
    393.600555f,
    399.948975f,
    406.505493f,
    413.280579f,
    420.285339f,
    427.531647f,
    435.032196f,
    442.800629f,
    450.851562f,
    459.200653f,
    467.864838f,
    476.862213f,
    486.212463f,
    495.936707f,
    506.057861f,
    516.600769f,
    527.592224f,
    539.061646f,
    551.040771f,
    563.564453f,
    576.670593f,
    590.400818f,
    604.800842f,
    619.920898f,
    635.816284f,
    652.548279f,
    670.184753f,
    688.800964f,
    708.481018f,
    729.318665f,
    751.419250f,
    774.901123f,
    799.897949f,
    826.561157f,
    855.063293f,
    885.601257f};
const float Copper_Eta[Copper_nSamples] = {
    1.400313f,
    1.380000f,
    1.358438f,
    1.340000f,
    1.329063f,
    1.325000f,
    1.332500f,
    1.340000f,
    1.334375f,
    1.325000f,
    1.317812f,
    1.310000f,
    1.300313f,
    1.290000f,
    1.281563f,
    1.270000f,
    1.249062f,
    1.225000f,
    1.200000f,
    1.180000f,
    1.174375f,
    1.175000f,
    1.177500f,
    1.180000f,
    1.178125f,
    1.175000f,
    1.172812f,
    1.170000f,
    1.165312f,
    1.160000f,
    1.155312f,
    1.150000f,
    1.142812f,
    1.135000f,
    1.131562f,
    1.120000f,
    1.092437f,
    1.040000f,
    0.950375f,
    0.826000f,
    0.645875f,
    0.468000f,
    0.351250f,
    0.272000f,
    0.230813f,
    0.214000f,
    0.209250f,
    0.213000f,
    0.216250f,
    0.223000f,
    0.236500f,
    0.250000f,
    0.254188f,
    0.260000f,
    0.280000f,
    0.300000f};
const float Copper_K[Copper_nSamples] = {
    1.662125f,
    1.687000f,
    1.703313f,
    1.720000f,
    1.744563f,
    1.770000f,
    1.791625f,
    1.810000f,
    1.822125f,
    1.834000f,
    1.851750f,
    1.872000f,
    1.894250f,
    1.916000f,
    1.931688f,
    1.950000f,
    1.972438f,
    2.015000f,
    2.121562f,
    2.210000f,
    2.177188f,
    2.130000f,
    2.160063f,
    2.210000f,
    2.249938f,
    2.289000f,
    2.326000f,
    2.362000f,
    2.397625f,
    2.433000f,
    2.469187f,
    2.504000f,
    2.535875f,
    2.564000f,
    2.589625f,
    2.605000f,
    2.595562f,
    2.583000f,
    2.576500f,
    2.599000f,
    2.678062f,
    2.809000f,
    3.010750f,
    3.240000f,
    3.458187f,
    3.670000f,
    3.863125f,
    4.050000f,
    4.239563f,
    4.430000f,
    4.619563f,
    4.817000f,
    5.034125f,
    5.260000f,
    5.485625f,
    5.717000f};

inline PrincipalSpectrum SilverEta, SilverK;
const int Silver_nSamples = 56;
const float Silver_Lambda[Silver_nSamples] = {
    298.757050f,
    302.400421f,
    306.133759f,
    309.960449f,
    313.884003f,
    317.908142f,
    322.036835f,
    326.274139f,
    330.624481f,
    335.092377f,
    339.682678f,
    344.400482f,
    349.251221f,
    354.240509f,
    359.374420f,
    364.659332f,
    370.102020f,
    375.709625f,
    381.489777f,
    387.450562f,
    393.600555f,
    399.948975f,
    406.505493f,
    413.280579f,
    420.285339f,
    427.531647f,
    435.032196f,
    442.800629f,
    450.851562f,
    459.200653f,
    467.864838f,
    476.862213f,
    486.212463f,
    495.936707f,
    506.057861f,
    516.600769f,
    527.592224f,
    539.061646f,
    551.040771f,
    563.564453f,
    576.670593f,
    590.400818f,
    604.800842f,
    619.920898f,
    635.816284f,
    652.548279f,
    670.184753f,
    688.800964f,
    708.481018f,
    729.318665f,
    751.419250f,
    774.901123f,
    799.897949f,
    826.561157f,
    855.063293f,
    885.601257f};
const float Silver_Eta[Silver_nSamples] = {
    1.519000f,
    1.496000f,
    1.432500f,
    1.323000f,
    1.142062f,
    0.932000f,
    0.719062f,
    0.526000f,
    0.388125f,
    0.294000f,
    0.253313f,
    0.238000f,
    0.221438f,
    0.209000f,
    0.194813f,
    0.186000f,
    0.192063f,
    0.200000f,
    0.198063f,
    0.192000f,
    0.182000f,
    0.173000f,
    0.172625f,
    0.173000f,
    0.166688f,
    0.160000f,
    0.158500f,
    0.157000f,
    0.151063f,
    0.144000f,
    0.137313f,
    0.132000f,
    0.130250f,
    0.130000f,
    0.129938f,
    0.130000f,
    0.130063f,
    0.129000f,
    0.124375f,
    0.120000f,
    0.119313f,
    0.121000f,
    0.125500f,
    0.131000f,
    0.136125f,
    0.140000f,
    0.140063f,
    0.140000f,
    0.144313f,
    0.148000f,
    0.145875f,
    0.143000f,
    0.142563f,
    0.145000f,
    0.151938f,
    0.163000f};
const float Silver_K[Silver_nSamples] = {
    1.080000f,
    0.882000f,
    0.761063f,
    0.647000f,
    0.550875f,
    0.504000f,
    0.554375f,
    0.663000f,
    0.818563f,
    0.986000f,
    1.120687f,
    1.240000f,
    1.345250f,
    1.440000f,
    1.533750f,
    1.610000f,
    1.641875f,
    1.670000f,
    1.735000f,
    1.810000f,
    1.878750f,
    1.950000f,
    2.029375f,
    2.110000f,
    2.186250f,
    2.260000f,
    2.329375f,
    2.400000f,
    2.478750f,
    2.560000f,
    2.640000f,
    2.720000f,
    2.798125f,
    2.880000f,
    2.973750f,
    3.070000f,
    3.159375f,
    3.250000f,
    3.348125f,
    3.450000f,
    3.553750f,
    3.660000f,
    3.766250f,
    3.880000f,
    4.010625f,
    4.150000f,
    4.293125f,
    4.440000f,
    4.586250f,
    4.740000f,
    4.908125f,
    5.090000f,
    5.288750f,
    5.500000f,
    5.720624f,
    5.950000f};

inline PrincipalSpectrum AluminiumEta, AluminiumK;
const int Aluminium_nSamples = 56;
const float Aluminium_Lambda[Aluminium_nSamples] = {
    298.757050f,
    302.400421f,
    306.133759f,
    309.960449f,
    313.884003f,
    317.908142f,
    322.036835f,
    326.274139f,
    330.624481f,
    335.092377f,
    339.682678f,
    344.400482f,
    349.251221f,
    354.240509f,
    359.374420f,
    364.659332f,
    370.102020f,
    375.709625f,
    381.489777f,
    387.450562f,
    393.600555f,
    399.948975f,
    406.505493f,
    413.280579f,
    420.285339f,
    427.531647f,
    435.032196f,
    442.800629f,
    450.851562f,
    459.200653f,
    467.864838f,
    476.862213f,
    486.212463f,
    495.936707f,
    506.057861f,
    516.600769f,
    527.592224f,
    539.061646f,
    551.040771f,
    563.564453f,
    576.670593f,
    590.400818f,
    604.800842f,
    619.920898f,
    635.816284f,
    652.548279f,
    670.184753f,
    688.800964f,
    708.481018f,
    729.318665f,
    751.419250f,
    774.901123f,
    799.897949f,
    826.561157f,
    855.063293f,
    885.601257f};
const float Aluminium_Eta[Aluminium_nSamples] = {
    0.273375f,
    0.280000f,
    0.286813f,
    0.294000f,
    0.301875f,
    0.310000f,
    0.317875f,
    0.326000f,
    0.334750f,
    0.344000f,
    0.353813f,
    0.364000f,
    0.374375f,
    0.385000f,
    0.395750f,
    0.407000f,
    0.419125f,
    0.432000f,
    0.445688f,
    0.460000f,
    0.474688f,
    0.490000f,
    0.506188f,
    0.523000f,
    0.540063f,
    0.558000f,
    0.577313f,
    0.598000f,
    0.620313f,
    0.644000f,
    0.668625f,
    0.695000f,
    0.723750f,
    0.755000f,
    0.789000f,
    0.826000f,
    0.867000f,
    0.912000f,
    0.963000f,
    1.020000f,
    1.080000f,
    1.150000f,
    1.220000f,
    1.300000f,
    1.390000f,
    1.490000f,
    1.600000f,
    1.740000f,
    1.910000f,
    2.140000f,
    2.410000f,
    2.630000f,
    2.800000f,
    2.740000f,
    2.580000f,
    2.240000f};
const float Aluminium_K[Aluminium_nSamples] = {
    3.593750f,
    3.640000f,
    3.689375f,
    3.740000f,
    3.789375f,
    3.840000f,
    3.894375f,
    3.950000f,
    4.005000f,
    4.060000f,
    4.113750f,
    4.170000f,
    4.233750f,
    4.300000f,
    4.365000f,
    4.430000f,
    4.493750f,
    4.560000f,
    4.633750f,
    4.710000f,
    4.784375f,
    4.860000f,
    4.938125f,
    5.020000f,
    5.108750f,
    5.200000f,
    5.290000f,
    5.380000f,
    5.480000f,
    5.580000f,
    5.690000f,
    5.800000f,
    5.915000f,
    6.030000f,
    6.150000f,
    6.280000f,
    6.420000f,
    6.550000f,
    6.700000f,
    6.850000f,
    7.000000f,
    7.150000f,
    7.310000f,
    7.480000f,
    7.650000f,
    7.820000f,
    8.010000f,
    8.210000f,
    8.390000f,
    8.570000f,
    8.620000f,
    8.600000f,
    8.450000f,
    8.310000f,
    8.210000f,
    8.210000f};

inline PrincipalSpectrum BerylliumEta, BerylliumK;
const int Beryllium_nSamples = 14;
const float Beryllium_Lambda[Beryllium_nSamples] = {
    310.000000f,
    326.300018f,
    344.399994f,
    364.600006f,
    387.399994f,
    413.300018f,
    442.800018f,
    476.800018f,
    516.600037f,
    563.500000f,
    619.900024f,
    688.799988f,
    774.900024f,
    885.600037f};
const float Beryllium_Eta[Beryllium_nSamples] = {
    2.470000f,
    2.550000f,
    2.640000f,
    2.730000f,
    2.840000f,
    2.950000f,
    3.070000f,
    3.190000f,
    3.300000f,
    3.390000f,
    3.460000f,
    3.470000f,
    3.440000f,
    3.350000f};
const float Beryllium_K[Beryllium_nSamples] = {
    3.080000f,
    3.080000f,
    3.080000f,
    3.100000f,
    3.120000f,
    3.140000f,
    3.160000f,
    3.160000f,
    3.180000f,
    3.170000f,
    3.180000f,
    3.230000f,
    3.350000f,
    3.550000f};

inline PrincipalSpectrum ChromiumEta, ChromiumK;
const int Chromium_nSamples = 28;
const float Chromium_Lambda[Chromium_nSamples] = {
    300.194000f,
    307.643005f,
    316.276001f,
    323.708008f,
    333.279999f,
    341.542999f,
    351.217987f,
    362.514984f,
    372.312012f,
    385.031006f,
    396.102020f,
    409.175018f,
    424.589020f,
    438.092010f,
    455.808990f,
    471.406982f,
    490.040009f,
    512.314026f,
    532.102966f,
    558.468018f,
    582.066040f,
    610.739014f,
    700.452026f,
    815.658020f,
    826.533020f,
    849.178040f,
    860.971985f,
    885.570984f};
const float Chromium_Eta[Chromium_nSamples] = {
    0.980000f,
    1.020000f,
    1.060000f,
    1.120000f,
    1.180000f,
    1.260000f,
    1.330000f,
    1.390000f,
    1.430000f,
    1.440000f,
    1.480000f,
    1.540000f,
    1.650000f,
    1.800000f,
    1.990000f,
    2.220000f,
    2.490000f,
    2.750000f,
    2.980000f,
    3.180000f,
    3.340000f,
    3.480000f,
    3.840000f,
    4.230000f,
    4.270000f,
    4.310000f,
    4.330000f,
    4.380000f};
const float Chromium_K[Chromium_nSamples] = {
    2.670000f,
    2.760000f,
    2.850000f,
    2.950000f,
    3.040000f,
    3.120000f,
    3.180000f,
    3.240000f,
    3.310000f,
    3.400000f,
    3.540000f,
    3.710000f,
    3.890000f,
    4.060000f,
    4.220000f,
    4.360000f,
    4.440000f,
    4.460000f,
    4.450000f,
    4.410000f,
    4.380000f,
    4.360000f,
    4.370000f,
    4.340000f,
    4.330000f,
    4.320000f,
    4.320000f,
    4.310000f};

inline PrincipalSpectrum MercuryEta, MercuryK;
const int Mercury_nSamples = 14;
const float Mercury_Lambda[Mercury_nSamples] = {
    309.950012f,
    326.263000f,
    344.389008f,
    364.647003f,
    387.437988f,
    413.266998f,
    442.785980f,
    476.846008f,
    516.583008f,
    563.545044f,
    619.900024f,
    688.778015f,
    774.875000f,
    885.570984f};
const float Mercury_Eta[Mercury_nSamples] = {
    0.542000f,
    0.589000f,
    0.644000f,
    0.713000f,
    0.798000f,
    0.898000f,
    1.027000f,
    1.186000f,
    1.384000f,
    1.620000f,
    1.910000f,
    2.284000f,
    2.746000f,
    3.324000f};
const float Mercury_K[Mercury_nSamples] = {
    2.502000f,
    2.665000f,
    2.860000f,
    3.074000f,
    3.294000f,
    3.538000f,
    3.802000f,
    4.090000f,
    4.407000f,
    4.751000f,
    5.150000f,
    5.582000f,
    6.054000f,
    6.558000f};

inline PrincipalSpectrum IridiumEta, IridiumK;
const int Iridium_nSamples = 28;
const float Iridium_Lambda[Iridium_nSamples] = {
    302.389984f,
    309.950012f,
    317.897003f,
    326.263000f,
    335.081024f,
    344.389008f,
    354.229004f,
    364.647003f,
    375.696991f,
    387.437988f,
    399.935028f,
    413.266998f,
    427.516998f,
    442.785980f,
    459.185028f,
    476.846008f,
    495.920013f,
    516.583008f,
    539.044006f,
    563.545044f,
    590.381042f,
    619.900024f,
    652.526001f,
    688.778015f,
    729.294006f,
    774.875000f,
    826.533020f,
    885.570984f};
const float Iridium_Eta[Iridium_nSamples] = {
    1.620000f,
    1.640000f,
    1.640000f,
    1.610000f,
    1.570000f,
    1.520000f,
    1.500000f,
    1.530000f,
    1.570000f,
    1.620000f,
    1.680000f,
    1.730000f,
    1.770000f,
    1.810000f,
    1.850000f,
    1.910000f,
    1.980000f,
    2.070000f,
    2.180000f,
    2.290000f,
    2.400000f,
    2.500000f,
    2.570000f,
    2.640000f,
    2.690000f,
    2.680000f,
    2.650000f,
    2.720000f};
const float Iridium_K[Iridium_nSamples] = {
    2.700000f,
    2.680000f,
    2.670000f,
    2.690000f,
    2.720000f,
    2.810000f,
    2.930000f,
    3.050000f,
    3.150000f,
    3.260000f,
    3.350000f,
    3.430000f,
    3.510000f,
    3.610000f,
    3.730000f,
    3.860000f,
    4.000000f,
    4.140000f,
    4.260000f,
    4.380000f,
    4.480000f,
    4.570000f,
    4.680000f,
    4.810000f,
    4.920000f,
    5.080000f,
    5.390000f,
    5.740000f};

inline PrincipalSpectrum LithiumEta, LithiumK;
const int Lithium_nSamples = 9;
const float Lithium_Lambda[Lithium_nSamples] = {
    306.123016f,
    330.613007f,
    359.362030f,
    393.587006f,
    435.018036f,
    486.196014f,
    551.022034f,
    635.795044f,
    751.393982f};
const float Lithium_Eta[Lithium_nSamples] = {
    0.346000f,
    0.345000f,
    0.334000f,
    0.304000f,
    0.247000f,
    0.217000f,
    0.206000f,
    0.221000f,
    0.265000f};
const float Lithium_K[Lithium_nSamples] = {
    1.210000f,
    1.320000f,
    1.450000f,
    1.600000f,
    1.820000f,
    2.110000f,
    2.480000f,
    2.940000f,
    3.550000f};

void InitializeForge();

std::pair<PrincipalSpectrum, PrincipalSpectrum> GetMetalCoefficientsFromForge(int value);
} // namespace filianore

#endif