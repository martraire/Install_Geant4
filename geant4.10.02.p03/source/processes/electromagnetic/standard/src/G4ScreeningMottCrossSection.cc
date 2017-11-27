//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//      G4ScreeningMottCrossSection.cc
//-------------------------------------------------------------------
//
// GEANT4 Class header file
//
// File name:    G4ScreeningMottCrossSection
//
// Author:      Cristina Consolandi
//
// Creation date: 20.10.2011  
//
// Modifications:
// 27-05-2012 Added Analytic Fitting to the Mott Cross Section by means of G4MottCoefficients class.
//
//
// Class Description:
//	Computation of electron Coulomb Scattering Cross Section.
//	Suitable for high energy electrons and light target materials. 
//
//      Reference:
//      M.J. Boschini et al.
//     "Non Ionizing Energy Loss induced by Electrons in the Space Environment"
//      Proc. of the 13th International Conference on Particle Physics and Advanced Technology 
//      (13th ICPPAT, Como 3-7/10/2011), World Scientific (Singapore).
//	Available at: http://arxiv.org/abs/1111.4042v4
//
//      1) Mott Differential Cross Section Approximation:  
//	   For Target material up to Z=92 (U):
//         As described in http://arxiv.org/abs/1111.4042v4 
//         par. 2.1 , eq. (16)-(17)
//         Else (Z>92):
//	   W. A. McKinley and H. Fashbach, Phys. Rev. 74, (1948) 1759.
//      2) Screening coefficient: 
//      vomn G. Moliere, Z. Naturforsh A2 (1947), 133-145; A3 (1948), 78.
//      3) Nuclear Form Factor: 
//      A.V. Butkevich et al. Nucl. Instr. and Meth. in Phys. Res. A 488 (2002), 282-294.
//
// -------------------------------------------------------------------------------------
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#include "G4ScreeningMottCrossSection.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4MottCoefficients.hh"
#include "Randomize.hh"
#include "G4Proton.hh"
#include "G4LossTableManager.hh"
#include "G4NucleiProperties.hh"
#include "G4Element.hh"
#include "G4UnitsTable.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

static G4double angle[DIM]={1e-07,1.02329e-07,1.04713e-07,1.07152e-07,1.09648e-07,1.12202e-07,1.14815e-07,1.1749e-07,1.20226e-07,1.23027e-07,1.25893e-07,1.28825e-07,1.31826e-07,1.34896e-07,1.38038e-07,1.41254e-07,1.44544e-07,1.47911e-07,1.51356e-07,1.54882e-07,1.58489e-07,1.62181e-07,1.65959e-07,1.69824e-07,1.7378e-07,1.77828e-07,1.8197e-07,1.86209e-07,1.90546e-07,1.94984e-07,1.99526e-07,2.04174e-07,2.0893e-07,2.13796e-07,2.18776e-07,2.23872e-07,2.29087e-07,2.34423e-07,2.39883e-07,2.45471e-07,2.51189e-07,2.5704e-07,2.63027e-07,2.69153e-07,2.75423e-07,2.81838e-07,2.88403e-07,2.95121e-07,3.01995e-07,3.0903e-07,3.16228e-07,3.23594e-07,3.31131e-07,3.38844e-07,3.46737e-07,3.54813e-07,3.63078e-07,3.71535e-07,3.80189e-07,3.89045e-07,3.98107e-07,4.0738e-07,4.16869e-07,4.2658e-07,4.36516e-07,4.46684e-07,4.57088e-07,4.67735e-07,4.7863e-07,4.89779e-07,5.01187e-07,5.12861e-07,5.24807e-07,5.37032e-07,5.49541e-07,5.62341e-07,5.7544e-07,5.88844e-07,6.0256e-07,6.16595e-07,6.30957e-07,6.45654e-07,6.60693e-07,6.76083e-07,6.91831e-07,7.07946e-07,7.24436e-07,7.4131e-07,7.58578e-07,7.76247e-07,7.94328e-07,8.12831e-07,8.31764e-07,8.51138e-07,8.70964e-07,8.91251e-07,9.12011e-07,9.33254e-07,9.54993e-07,9.77237e-07,1e-06,1.02329e-06,1.04713e-06,1.07152e-06,1.09648e-06,1.12202e-06,1.14815e-06,1.1749e-06,1.20226e-06,1.23027e-06,1.25893e-06,1.28825e-06,1.31826e-06,1.34896e-06,1.38038e-06,1.41254e-06,1.44544e-06,1.47911e-06,1.51356e-06,1.54882e-06,1.58489e-06,1.62181e-06,1.65959e-06,1.69824e-06,1.7378e-06,1.77828e-06,1.8197e-06,1.86209e-06,1.90546e-06,1.94984e-06,1.99526e-06,2.04174e-06,2.0893e-06,2.13796e-06,2.18776e-06,2.23872e-06,2.29087e-06,2.34423e-06,2.39883e-06,2.45471e-06,2.51189e-06,2.5704e-06,2.63027e-06,2.69153e-06,2.75423e-06,2.81838e-06,2.88403e-06,2.95121e-06,3.01995e-06,3.0903e-06,3.16228e-06,3.23594e-06,3.31131e-06,3.38844e-06,3.46737e-06,3.54813e-06,3.63078e-06,3.71535e-06,3.80189e-06,3.89045e-06,3.98107e-06,4.0738e-06,4.16869e-06,4.2658e-06,4.36516e-06,4.46684e-06,4.57088e-06,4.67735e-06,4.7863e-06,4.89779e-06,5.01187e-06,5.12861e-06,5.24807e-06,5.37032e-06,5.49541e-06,5.62341e-06,5.7544e-06,5.88844e-06,6.0256e-06,6.16595e-06,6.30957e-06,6.45654e-06,6.60693e-06,6.76083e-06,6.91831e-06,7.07946e-06,7.24436e-06,7.4131e-06,7.58578e-06,7.76247e-06,7.94328e-06,8.12831e-06,8.31764e-06,8.51138e-06,8.70964e-06,8.91251e-06,9.12011e-06,9.33254e-06,9.54993e-06,9.77237e-06,1e-05,1.02329e-05,1.04713e-05,1.07152e-05,1.09648e-05,1.12202e-05,1.14815e-05,1.1749e-05,1.20226e-05,1.23027e-05,1.25893e-05,1.28825e-05,1.31826e-05,1.34896e-05,1.38038e-05,1.41254e-05,1.44544e-05,1.47911e-05,1.51356e-05,1.54882e-05,1.58489e-05,1.62181e-05,1.65959e-05,1.69824e-05,1.7378e-05,1.77828e-05,1.8197e-05,1.86209e-05,1.90546e-05,1.94984e-05,1.99526e-05,2.04174e-05,2.0893e-05,2.13796e-05,2.18776e-05,2.23872e-05,2.29087e-05,2.34423e-05,2.39883e-05,2.45471e-05,2.51189e-05,2.5704e-05,2.63027e-05,2.69153e-05,2.75423e-05,2.81838e-05,2.88403e-05,2.95121e-05,3.01995e-05,3.0903e-05,3.16228e-05,3.23594e-05,3.31131e-05,3.38844e-05,3.46737e-05,3.54813e-05,3.63078e-05,3.71535e-05,3.80189e-05,3.89045e-05,3.98107e-05,4.0738e-05,4.16869e-05,4.2658e-05,4.36516e-05,4.46684e-05,4.57088e-05,4.67735e-05,4.7863e-05,4.89779e-05,5.01187e-05,5.12861e-05,5.24807e-05,5.37032e-05,5.49541e-05,5.62341e-05,5.7544e-05,5.88844e-05,6.0256e-05,6.16595e-05,6.30957e-05,6.45654e-05,6.60693e-05,6.76083e-05,6.91831e-05,7.07946e-05,7.24436e-05,7.4131e-05,7.58578e-05,7.76247e-05,7.94328e-05,8.12831e-05,8.31764e-05,8.51138e-05,8.70964e-05,8.91251e-05,9.12011e-05,9.33254e-05,9.54993e-05,9.77237e-05,0.0001,0.000102329,0.000104713,0.000107152,0.000109648,0.000112202,0.000114815,0.00011749,0.000120226,0.000123027,0.000125893,0.000128825,0.000131826,0.000134896,0.000138038,0.000141254,0.000144544,0.000147911,0.000151356,0.000154882,0.000158489,0.000162181,0.000165959,0.000169824,0.00017378,0.000177828,0.00018197,0.000186209,0.000190546,0.000194984,0.000199526,0.000204174,0.00020893,0.000213796,0.000218776,0.000223872,0.000229087,0.000234423,0.000239883,0.000245471,0.000251189,0.00025704,0.000263027,0.000269153,0.000275423,0.000281838,0.000288403,0.000295121,0.000301995,0.00030903,0.000316228,0.000323594,0.000331131,0.000338844,0.000346737,0.000354813,0.000363078,0.000371535,0.000380189,0.000389045,0.000398107,0.00040738,0.000416869,0.00042658,0.000436516,0.000446684,0.000457088,0.000467735,0.00047863,0.000489779,0.000501187,0.000512861,0.000524807,0.000537032,0.000549541,0.000562341,0.00057544,0.000588844,0.00060256,0.000616595,0.000630957,0.000645654,0.000660693,0.000676083,0.000691831,0.000707946,0.000724436,0.00074131,0.000758578,0.000776247,0.000794328,0.000812831,0.000831764,0.000851138,0.000870964,0.000891251,0.000912011,0.000933254,0.000954993,0.000977237,0.001,0.00102329,0.00104713,0.00107152,0.00109648,0.00112202,0.00114815,0.0011749,0.00120226,0.00123027,0.00125893,0.00128825,0.00131826,0.00134896,0.00138038,0.00141254,0.00144544,0.00147911,0.00151356,0.00154882,0.00158489,0.00162181,0.00165959,0.00169824,0.0017378,0.00177828,0.0018197,0.00186209,0.00190546,0.00194984,0.00199526,0.00204174,0.0020893,0.00213796,0.00218776,0.00223872,0.00229087,0.00234423,0.00239883,0.00245471,0.00251189,0.0025704,0.00263027,0.00269153,0.00275423,0.00281838,0.00288403,0.00295121,0.00301995,0.0030903,0.00316228,0.00323594,0.00331131,0.00338844,0.00346737,0.00354813,0.00363078,0.00371535,0.00380189,0.00389045,0.00398107,0.0040738,0.00416869,0.0042658,0.00436516,0.00446684,0.00457088,0.00467735,0.0047863,0.00489779,0.00501187,0.00512861,0.00524807,0.00537032,0.00549541,0.00562341,0.0057544,0.00588844,0.0060256,0.00616595,0.00630957,0.00645654,0.00660693,0.00676083,0.00691831,0.00707946,0.00724436,0.0074131,0.00758578,0.00776247,0.00794328,0.00812831,0.00831764,0.00851138,0.00870964,0.00891251,0.00912011,0.00933254,0.00954993,0.00977237,0.01,0.0102329,0.0104713,0.0107152,0.0109648,0.0112202,0.0114815,0.011749,0.0120226,0.0123027,0.0125893,0.0128825,0.0131826,0.0134896,0.0138038,0.0141254,0.0144544,0.0147911,0.0151356,0.0154882,0.0158489,0.0162181,0.0165959,0.0169824,0.017378,0.0177828,0.018197,0.0186209,0.0190546,0.0194984,0.0199526,0.0204174,0.020893,0.0213796,0.0218776,0.0223872,0.0229087,0.0234423,0.0239883,0.0245471,0.0251189,0.025704,0.0263027,0.0269153,0.0275423,0.0281838,0.0288403,0.0295121,0.0301995,0.030903,0.0316228,0.0323594,0.0331131,0.0338844,0.0346737,0.0354813,0.0363078,0.0371535,0.0380189,0.0389045,0.0398107,0.040738,0.0416869,0.042658,0.0436516,0.0446684,0.0457088,0.0467735,0.047863,0.0489779,0.0501187,0.0512861,0.0524807,0.0537032,0.0549541,0.0562341,0.057544,0.0588844,0.060256,0.0616595,0.0630957,0.0645654,0.0660693,0.0676083,0.0691831,0.0707946,0.0724436,0.074131,0.0758578,0.0776247,0.0794328,0.0812831,0.0831764,0.0851138,0.0870964,0.0891251,0.0912011,0.0933254,0.0954993,0.0977237,0.1,0.102329,0.104713,0.107152,0.109648,0.112202,0.114815,0.11749,0.120226,0.123027,0.125893,0.128825,0.131826,0.134896,0.138038,0.141254,0.144544,0.147911,0.151356,0.154882,0.158489,0.162181,0.165959,0.169824,0.17378,0.177828,0.18197,0.186209,0.190546,0.194984,0.199526,0.204174,0.20893,0.213796,0.218776,0.223872,0.229087,0.234423,0.239883,0.245471,0.251189,0.25704,0.263027,0.269153,0.275423,0.281838,0.288403,0.295121,0.301995,0.30903,0.316228,0.323594,0.331131,0.338844,0.346737,0.354813,0.363078,0.371535,0.380189,0.389045,0.398107,0.40738,0.416869,0.42658,0.436516,0.446684,0.457088,0.467735,0.47863,0.489779,0.501187,0.512861,0.524807,0.537032,0.549541,0.562341,0.57544,0.588844,0.60256,0.616595,0.630957,0.645654,0.660693,0.676083,0.691831,0.707946,0.724436,0.74131,0.758578,0.776247,0.794328,0.812831,0.831764,0.851138,0.870964,0.891251,0.912011,0.933254,0.954993,0.977237,1,1.02329,1.04713,1.07152,1.09648,1.12202,1.14815,1.1749,1.20226,1.23027,1.25893,1.28825,1.31826,1.34896,1.38038,1.41254,1.44544,1.47911,1.51356,1.54882,1.58489,1.62181,1.65959,1.69824,1.7378,1.77828,1.8197,1.86209,1.90546,1.94984,1.99526,2.04174,2.0893,2.13796,2.18776,2.23872,2.29087,2.34423,2.39883,2.45471,2.51189,2.5704,2.63027,2.69153,2.75423,2.81838,2.88403,2.95121,3.01995,3.0903};
static G4double dangle[DIM]={2.329e-09,2.384e-09,2.439e-09,2.496e-09,2.554e-09,2.613e-09,2.675e-09,2.736e-09,2.801e-09,2.866e-09,2.932e-09,3.001e-09,3.07e-09,3.142e-09,3.216e-09,3.29e-09,3.367e-09,3.445e-09,3.526e-09,3.607e-09,3.692e-09,3.778e-09,3.865e-09,3.956e-09,4.048e-09,4.142e-09,4.239e-09,4.337e-09,4.438e-09,4.542e-09,4.648e-09,4.756e-09,4.866e-09,4.98e-09,5.096e-09,5.215e-09,5.336e-09,5.46e-09,5.588e-09,5.718e-09,5.851e-09,5.987e-09,6.126e-09,6.27e-09,6.415e-09,6.565e-09,6.718e-09,6.874e-09,7.035e-09,7.198e-09,7.366e-09,7.537e-09,7.713e-09,7.893e-09,8.076e-09,8.265e-09,8.457e-09,8.654e-09,8.856e-09,9.062e-09,9.273e-09,9.489e-09,9.711e-09,9.936e-09,1.0168e-08,1.0404e-08,1.0647e-08,1.0895e-08,1.1149e-08,1.1408e-08,1.1674e-08,1.1946e-08,1.2225e-08,1.2509e-08,1.28e-08,1.3099e-08,1.3404e-08,1.3716e-08,1.4035e-08,1.4362e-08,1.4697e-08,1.5039e-08,1.539e-08,1.5748e-08,1.6115e-08,1.649e-08,1.6874e-08,1.7268e-08,1.7669e-08,1.8081e-08,1.8503e-08,1.8933e-08,1.9374e-08,1.9826e-08,2.0287e-08,2.076e-08,2.1243e-08,2.1739e-08,2.2244e-08,2.2763e-08,2.329e-08,2.384e-08,2.439e-08,2.496e-08,2.554e-08,2.613e-08,2.675e-08,2.736e-08,2.801e-08,2.866e-08,2.932e-08,3.001e-08,3.07e-08,3.142e-08,3.216e-08,3.29e-08,3.367e-08,3.445e-08,3.526e-08,3.607e-08,3.692e-08,3.778e-08,3.865e-08,3.956e-08,4.048e-08,4.142e-08,4.239e-08,4.337e-08,4.438e-08,4.542e-08,4.648e-08,4.756e-08,4.866e-08,4.98e-08,5.096e-08,5.215e-08,5.336e-08,5.46e-08,5.588e-08,5.718e-08,5.851e-08,5.987e-08,6.126e-08,6.27e-08,6.415e-08,6.565e-08,6.718e-08,6.874e-08,7.035e-08,7.198e-08,7.366e-08,7.537e-08,7.713e-08,7.893e-08,8.076e-08,8.265e-08,8.457e-08,8.654e-08,8.856e-08,9.062e-08,9.273e-08,9.489e-08,9.711e-08,9.936e-08,1.0168e-07,1.0404e-07,1.0647e-07,1.0895e-07,1.1149e-07,1.1408e-07,1.1674e-07,1.1946e-07,1.2225e-07,1.2509e-07,1.28e-07,1.3099e-07,1.3404e-07,1.3716e-07,1.4035e-07,1.4362e-07,1.4697e-07,1.5039e-07,1.539e-07,1.5748e-07,1.6115e-07,1.649e-07,1.6874e-07,1.7268e-07,1.7669e-07,1.8081e-07,1.8503e-07,1.8933e-07,1.9374e-07,1.9826e-07,2.0287e-07,2.076e-07,2.1243e-07,2.1739e-07,2.2244e-07,2.2763e-07,2.329e-07,2.384e-07,2.439e-07,2.496e-07,2.554e-07,2.613e-07,2.675e-07,2.736e-07,2.801e-07,2.866e-07,2.932e-07,3.001e-07,3.07e-07,3.142e-07,3.216e-07,3.29e-07,3.367e-07,3.445e-07,3.526e-07,3.607e-07,3.692e-07,3.778e-07,3.865e-07,3.956e-07,4.048e-07,4.142e-07,4.239e-07,4.337e-07,4.438e-07,4.542e-07,4.648e-07,4.756e-07,4.866e-07,4.98e-07,5.096e-07,5.215e-07,5.336e-07,5.46e-07,5.588e-07,5.718e-07,5.851e-07,5.987e-07,6.126e-07,6.27e-07,6.415e-07,6.565e-07,6.718e-07,6.874e-07,7.035e-07,7.198e-07,7.366e-07,7.537e-07,7.713e-07,7.893e-07,8.076e-07,8.265e-07,8.457e-07,8.654e-07,8.856e-07,9.062e-07,9.273e-07,9.489e-07,9.711e-07,9.936e-07,1.0168e-06,1.0404e-06,1.0647e-06,1.0895e-06,1.1149e-06,1.1408e-06,1.1674e-06,1.1946e-06,1.2225e-06,1.2509e-06,1.28e-06,1.3099e-06,1.3404e-06,1.3716e-06,1.4035e-06,1.4362e-06,1.4697e-06,1.5039e-06,1.539e-06,1.5748e-06,1.6115e-06,1.649e-06,1.6874e-06,1.7268e-06,1.7669e-06,1.8081e-06,1.8503e-06,1.8933e-06,1.9374e-06,1.9826e-06,2.0287e-06,2.076e-06,2.1243e-06,2.1739e-06,2.2244e-06,2.2763e-06,2.329e-06,2.384e-06,2.439e-06,2.496e-06,2.554e-06,2.613e-06,2.675e-06,2.736e-06,2.801e-06,2.866e-06,2.932e-06,3.001e-06,3.07e-06,3.142e-06,3.216e-06,3.29e-06,3.367e-06,3.445e-06,3.526e-06,3.607e-06,3.692e-06,3.778e-06,3.865e-06,3.956e-06,4.048e-06,4.142e-06,4.239e-06,4.337e-06,4.438e-06,4.542e-06,4.648e-06,4.756e-06,4.866e-06,4.98e-06,5.096e-06,5.215e-06,5.336e-06,5.46e-06,5.588e-06,5.718e-06,5.851e-06,5.987e-06,6.126e-06,6.27e-06,6.415e-06,6.565e-06,6.718e-06,6.874e-06,7.035e-06,7.198e-06,7.366e-06,7.537e-06,7.713e-06,7.893e-06,8.076e-06,8.265e-06,8.457e-06,8.654e-06,8.856e-06,9.062e-06,9.273e-06,9.489e-06,9.711e-06,9.936e-06,1.0168e-05,1.0404e-05,1.0647e-05,1.0895e-05,1.1149e-05,1.1408e-05,1.1674e-05,1.1946e-05,1.2225e-05,1.2509e-05,1.28e-05,1.3099e-05,1.3404e-05,1.3716e-05,1.4035e-05,1.4362e-05,1.4697e-05,1.5039e-05,1.539e-05,1.5748e-05,1.6115e-05,1.649e-05,1.6874e-05,1.7268e-05,1.7669e-05,1.8081e-05,1.8503e-05,1.8933e-05,1.9374e-05,1.9826e-05,2.0287e-05,2.076e-05,2.1243e-05,2.1739e-05,2.2244e-05,2.2763e-05,2.329e-05,2.384e-05,2.439e-05,2.496e-05,2.554e-05,2.613e-05,2.675e-05,2.736e-05,2.801e-05,2.866e-05,2.932e-05,3.001e-05,3.07e-05,3.142e-05,3.216e-05,3.29e-05,3.367e-05,3.445e-05,3.526e-05,3.607e-05,3.692e-05,3.778e-05,3.865e-05,3.956e-05,4.048e-05,4.142e-05,4.239e-05,4.337e-05,4.438e-05,4.542e-05,4.648e-05,4.756e-05,4.866e-05,4.98e-05,5.096e-05,5.215e-05,5.336e-05,5.46e-05,5.588e-05,5.718e-05,5.851e-05,5.987e-05,6.126e-05,6.27e-05,6.415e-05,6.565e-05,6.718e-05,6.874e-05,7.035e-05,7.198e-05,7.366e-05,7.537e-05,7.713e-05,7.893e-05,8.076e-05,8.265e-05,8.457e-05,8.654e-05,8.856e-05,9.062e-05,9.273e-05,9.489e-05,9.711e-05,9.936e-05,0.00010168,0.00010404,0.00010647,0.00010895,0.00011149,0.00011408,0.00011674,0.00011946,0.00012225,0.00012509,0.000128,0.00013099,0.00013404,0.00013716,0.00014035,0.00014362,0.00014697,0.00015039,0.0001539,0.00015748,0.00016115,0.0001649,0.00016874,0.00017268,0.00017669,0.00018081,0.00018503,0.00018933,0.00019374,0.00019826,0.00020287,0.0002076,0.00021243,0.00021739,0.00022244,0.00022763,0.0002329,0.0002384,0.0002439,0.0002496,0.0002554,0.0002613,0.0002675,0.0002736,0.0002801,0.0002866,0.0002932,0.0003001,0.000307,0.0003142,0.0003216,0.000329,0.0003367,0.0003445,0.0003526,0.0003607,0.0003692,0.0003778,0.0003865,0.0003956,0.0004048,0.0004142,0.0004239,0.0004337,0.0004438,0.0004542,0.0004648,0.0004756,0.0004866,0.000498,0.0005096,0.0005215,0.0005336,0.000546,0.0005588,0.0005718,0.0005851,0.0005987,0.0006126,0.000627,0.0006415,0.0006565,0.0006718,0.0006874,0.0007035,0.0007198,0.0007366,0.0007537,0.0007713,0.0007893,0.0008076,0.0008265,0.0008457,0.0008654,0.0008856,0.0009062,0.0009273,0.0009489,0.0009711,0.0009936,0.0010168,0.0010404,0.0010647,0.0010895,0.0011149,0.0011408,0.0011674,0.0011946,0.0012225,0.0012509,0.00128,0.0013099,0.0013404,0.0013716,0.0014035,0.0014362,0.0014697,0.0015039,0.001539,0.0015748,0.0016115,0.001649,0.0016874,0.0017268,0.0017669,0.0018081,0.0018503,0.0018933,0.0019374,0.0019826,0.0020287,0.002076,0.0021243,0.0021739,0.0022244,0.0022763,0.002329,0.002384,0.002439,0.002496,0.002554,0.002613,0.002675,0.002736,0.002801,0.002866,0.002932,0.003001,0.00307,0.003142,0.003216,0.00329,0.003367,0.003445,0.003526,0.003607,0.003692,0.003778,0.003865,0.003956,0.004048,0.004142,0.004239,0.004337,0.004438,0.004542,0.004648,0.004756,0.004866,0.00498,0.005096,0.005215,0.005336,0.00546,0.005588,0.005718,0.005851,0.005987,0.006126,0.00627,0.006415,0.006565,0.006718,0.006874,0.007035,0.007198,0.007366,0.007537,0.007713,0.007893,0.008076,0.008265,0.008457,0.008654,0.008856,0.009062,0.009273,0.009489,0.009711,0.009936,0.010168,0.010404,0.010647,0.010895,0.011149,0.011408,0.011674,0.011946,0.012225,0.012509,0.0128,0.013099,0.013404,0.013716,0.014035,0.014362,0.014697,0.015039,0.01539,0.015748,0.016115,0.01649,0.016874,0.017268,0.017669,0.018081,0.018503,0.018933,0.019374,0.019826,0.020287,0.02076,0.021243,0.021739,0.022244,0.022763,0.02329,0.02384,0.02439,0.02496,0.02554,0.02613,0.02675,0.02736,0.02801,0.02866,0.02932,0.03001,0.0307,0.03142,0.03216,0.0329,0.03367,0.03445,0.03526,0.03607,0.03692,0.03778,0.03865,0.03956,0.04048,0.04142,0.04239,0.04337,0.04438,0.04542,0.04648,0.04756,0.04866,0.0498,0.05096,0.05215,0.05336,0.0546,0.05588,0.05718,0.05851,0.05987,0.06126,0.0627,0.06415,0.06565,0.06718,0.06874,0.07035,0.051293};
static G4double tet[DIM]={1.01165e-07,1.03521e-07,1.05933e-07,1.084e-07,1.10925e-07,1.13509e-07,1.16153e-07,1.18858e-07,1.21627e-07,1.2446e-07,1.27359e-07,1.30326e-07,1.33361e-07,1.36467e-07,1.39646e-07,1.42899e-07,1.46227e-07,1.49633e-07,1.53119e-07,1.56685e-07,1.60335e-07,1.6407e-07,1.67891e-07,1.71802e-07,1.75804e-07,1.79899e-07,1.84089e-07,1.88377e-07,1.92765e-07,1.97255e-07,2.0185e-07,2.06552e-07,2.11363e-07,2.16286e-07,2.21324e-07,2.26479e-07,2.31755e-07,2.37153e-07,2.42677e-07,2.4833e-07,2.54115e-07,2.60034e-07,2.6609e-07,2.72288e-07,2.7863e-07,2.8512e-07,2.91762e-07,2.98558e-07,3.05513e-07,3.12629e-07,3.19911e-07,3.27363e-07,3.34987e-07,3.4279e-07,3.50775e-07,3.58946e-07,3.67307e-07,3.75862e-07,3.84617e-07,3.93576e-07,4.02744e-07,4.12124e-07,4.21725e-07,4.31548e-07,4.416e-07,4.51886e-07,4.62412e-07,4.73183e-07,4.84204e-07,4.95483e-07,5.07024e-07,5.18834e-07,5.3092e-07,5.43286e-07,5.55941e-07,5.68891e-07,5.82142e-07,5.95702e-07,6.09577e-07,6.23776e-07,6.38305e-07,6.53173e-07,6.68388e-07,6.83957e-07,6.99889e-07,7.16191e-07,7.32873e-07,7.49944e-07,7.67412e-07,7.85287e-07,8.03579e-07,8.22297e-07,8.41451e-07,8.61051e-07,8.81108e-07,9.01631e-07,9.22633e-07,9.44124e-07,9.66115e-07,9.88618e-07,1.01164e-06,1.03521e-06,1.05933e-06,1.084e-06,1.10925e-06,1.13508e-06,1.16152e-06,1.18858e-06,1.21626e-06,1.2446e-06,1.27359e-06,1.30325e-06,1.33361e-06,1.36467e-06,1.39646e-06,1.42899e-06,1.46227e-06,1.49633e-06,1.53119e-06,1.56686e-06,1.60335e-06,1.6407e-06,1.67891e-06,1.71802e-06,1.75804e-06,1.79899e-06,1.8409e-06,1.88378e-06,1.92765e-06,1.97255e-06,2.0185e-06,2.06552e-06,2.11363e-06,2.16286e-06,2.21324e-06,2.26479e-06,2.31755e-06,2.37153e-06,2.42677e-06,2.4833e-06,2.54114e-06,2.60034e-06,2.6609e-06,2.72288e-06,2.7863e-06,2.8512e-06,2.91762e-06,2.98558e-06,3.05512e-06,3.12629e-06,3.19911e-06,3.27362e-06,3.34987e-06,3.42791e-06,3.50775e-06,3.58945e-06,3.67306e-06,3.75862e-06,3.84617e-06,3.93576e-06,4.02744e-06,4.12125e-06,4.21724e-06,4.31548e-06,4.416e-06,4.51886e-06,4.62411e-06,4.73183e-06,4.84205e-06,4.95483e-06,5.07024e-06,5.18834e-06,5.30919e-06,5.43286e-06,5.55941e-06,5.68891e-06,5.82142e-06,5.95702e-06,6.09578e-06,6.23776e-06,6.38306e-06,6.53173e-06,6.68388e-06,6.83957e-06,6.99889e-06,7.16191e-06,7.32873e-06,7.49944e-06,7.67412e-06,7.85287e-06,8.03579e-06,8.22298e-06,8.41451e-06,8.61051e-06,8.81107e-06,9.01631e-06,9.22633e-06,9.44123e-06,9.66115e-06,9.88618e-06,1.01165e-05,1.03521e-05,1.05932e-05,1.084e-05,1.10925e-05,1.13508e-05,1.16153e-05,1.18858e-05,1.21627e-05,1.2446e-05,1.27359e-05,1.30326e-05,1.33361e-05,1.36467e-05,1.39646e-05,1.42899e-05,1.46227e-05,1.49633e-05,1.53119e-05,1.56685e-05,1.60335e-05,1.6407e-05,1.67892e-05,1.71802e-05,1.75804e-05,1.79899e-05,1.84089e-05,1.88378e-05,1.92765e-05,1.97255e-05,2.0185e-05,2.06552e-05,2.11363e-05,2.16286e-05,2.21324e-05,2.2648e-05,2.31755e-05,2.37153e-05,2.42677e-05,2.4833e-05,2.54115e-05,2.60033e-05,2.6609e-05,2.72288e-05,2.7863e-05,2.8512e-05,2.91762e-05,2.98558e-05,3.05513e-05,3.12629e-05,3.19911e-05,3.27362e-05,3.34987e-05,3.4279e-05,3.50775e-05,3.58946e-05,3.67307e-05,3.75862e-05,3.84617e-05,3.93576e-05,4.02743e-05,4.12125e-05,4.21724e-05,4.31548e-05,4.416e-05,4.51886e-05,4.62412e-05,4.73183e-05,4.84204e-05,4.95483e-05,5.07024e-05,5.18834e-05,5.30919e-05,5.43287e-05,5.55941e-05,5.68891e-05,5.82142e-05,5.95702e-05,6.09577e-05,6.23776e-05,6.38306e-05,6.53173e-05,6.68388e-05,6.83957e-05,6.99888e-05,7.16191e-05,7.32873e-05,7.49944e-05,7.67412e-05,7.85288e-05,8.0358e-05,8.22297e-05,8.41451e-05,8.61051e-05,8.81108e-05,9.01631e-05,9.22633e-05,9.44124e-05,9.66115e-05,9.88618e-05,0.000101165,0.000103521,0.000105932,0.0001084,0.000110925,0.000113508,0.000116153,0.000118858,0.000121626,0.00012446,0.000127359,0.000130325,0.000133361,0.000136467,0.000139646,0.000142899,0.000146228,0.000149633,0.000153119,0.000156686,0.000160335,0.00016407,0.000167891,0.000171802,0.000175804,0.000179899,0.000184089,0.000188377,0.000192765,0.000197255,0.00020185,0.000206552,0.000211363,0.000216286,0.000221324,0.000226479,0.000231755,0.000237153,0.000242677,0.00024833,0.000254115,0.000260033,0.00026609,0.000272288,0.000278631,0.00028512,0.000291762,0.000298558,0.000305513,0.000312629,0.000319911,0.000327363,0.000334987,0.00034279,0.000350775,0.000358946,0.000367307,0.000375862,0.000384617,0.000393576,0.000402743,0.000412124,0.000421725,0.000431548,0.0004416,0.000451886,0.000462411,0.000473183,0.000484204,0.000495483,0.000507024,0.000518834,0.000530919,0.000543286,0.000555941,0.000568891,0.000582142,0.000595702,0.000609577,0.000623776,0.000638305,0.000653173,0.000668388,0.000683957,0.000699889,0.000716191,0.000732873,0.000749944,0.000767413,0.000785287,0.000803579,0.000822297,0.000841451,0.000861051,0.000881108,0.000901631,0.000922633,0.000944124,0.000966115,0.000988618,0.00101165,0.00103521,0.00105933,0.001084,0.00110925,0.00113509,0.00116153,0.00118858,0.00121627,0.0012446,0.00127359,0.00130325,0.00133361,0.00136467,0.00139646,0.00142899,0.00146228,0.00149634,0.00153119,0.00156686,0.00160335,0.0016407,0.00167892,0.00171802,0.00175804,0.00179899,0.00184089,0.00188378,0.00192765,0.00197255,0.0020185,0.00206552,0.00211363,0.00216286,0.00221324,0.0022648,0.00231755,0.00237153,0.00242677,0.0024833,0.00254114,0.00260034,0.0026609,0.00272288,0.00278631,0.0028512,0.00291762,0.00298558,0.00305512,0.00312629,0.00319911,0.00327363,0.00334987,0.00342791,0.00350775,0.00358946,0.00367307,0.00375862,0.00384617,0.00393576,0.00402743,0.00412124,0.00421724,0.00431548,0.004416,0.00451886,0.00462411,0.00473183,0.00484204,0.00495483,0.00507024,0.00518834,0.00530919,0.00543287,0.00555941,0.0056889,0.00582142,0.00595702,0.00609577,0.00623776,0.00638306,0.00653174,0.00668388,0.00683957,0.00699888,0.00716191,0.00732873,0.00749944,0.00767413,0.00785288,0.0080358,0.00822298,0.00841451,0.00861051,0.00881107,0.00901631,0.00922633,0.00944123,0.00966115,0.00988619,0.0101164,0.0103521,0.0105932,0.01084,0.0110925,0.0113508,0.0116153,0.0118858,0.0121627,0.012446,0.0127359,0.0130326,0.0133361,0.0136467,0.0139646,0.0142899,0.0146228,0.0149634,0.0153119,0.0156685,0.0160335,0.016407,0.0167892,0.0171802,0.0175804,0.0179899,0.018409,0.0188378,0.0192765,0.0197255,0.020185,0.0206552,0.0211363,0.0216286,0.0221324,0.022648,0.0231755,0.0237153,0.0242677,0.024833,0.0254115,0.0260034,0.026609,0.0272288,0.0278631,0.028512,0.0291762,0.0298558,0.0305513,0.0312629,0.0319911,0.0327363,0.0334988,0.0342791,0.0350775,0.0358945,0.0367307,0.0375862,0.0384617,0.0393576,0.0402744,0.0412124,0.0421725,0.0431548,0.04416,0.0451886,0.0462412,0.0473183,0.0484205,0.0495483,0.0507024,0.0518834,0.0530919,0.0543286,0.0555941,0.056889,0.0582142,0.0595702,0.0609577,0.0623776,0.0638305,0.0653173,0.0668388,0.0683957,0.0699888,0.0716191,0.0732873,0.0749944,0.0767413,0.0785288,0.0803579,0.0822297,0.0841451,0.0861051,0.0881108,0.0901631,0.0922632,0.0944124,0.0966115,0.0988619,0.101165,0.103521,0.105932,0.1084,0.110925,0.113508,0.116152,0.118858,0.121626,0.12446,0.127359,0.130325,0.133361,0.136467,0.139646,0.142899,0.146228,0.149633,0.153119,0.156685,0.160335,0.16407,0.167891,0.171802,0.175804,0.179899,0.18409,0.188378,0.192765,0.197255,0.20185,0.206552,0.211363,0.216286,0.221324,0.22648,0.231755,0.237153,0.242677,0.24833,0.254115,0.260034,0.26609,0.272288,0.278631,0.28512,0.291762,0.298558,0.305513,0.312629,0.319911,0.327363,0.334987,0.34279,0.350775,0.358946,0.367306,0.375862,0.384617,0.393576,0.402744,0.412125,0.421725,0.431548,0.4416,0.451886,0.462411,0.473183,0.484205,0.495483,0.507024,0.518834,0.530919,0.543287,0.555941,0.56889,0.582142,0.595702,0.609577,0.623776,0.638305,0.653173,0.668388,0.683957,0.699888,0.716191,0.732873,0.749944,0.767413,0.785288,0.80358,0.822297,0.841451,0.861051,0.881107,0.901631,0.922632,0.944124,0.966115,0.988619,1.01165,1.03521,1.05932,1.084,1.10925,1.13509,1.16153,1.18858,1.21626,1.2446,1.27359,1.30326,1.33361,1.36467,1.39646,1.42899,1.46227,1.49633,1.53119,1.56685,1.60335,1.6407,1.67891,1.71802,1.75804,1.79899,1.8409,1.88377,1.92765,1.97255,2.0185,2.06552,2.11363,2.16286,2.21324,2.26479,2.31755,2.37153,2.42677,2.4833,2.54115,2.60033,2.6609,2.72288,2.78631,2.85121,2.91762,2.98558,3.05513,3.11595};

using namespace std;

G4ScreeningMottCrossSection::G4ScreeningMottCrossSection():
   cosThetaMin(1.0),
   cosThetaMax(-1.0),
   alpha(fine_structure_const),
   htc2(hbarc_squared),
   e2(electron_mass_c2*classic_electr_radius) 
{
  TotalCross=0;

  fNistManager = G4NistManager::Instance();
  fG4pow = G4Pow::GetInstance();
  particle=0;

  spin = mass = mu_rel=0;
  tkinLab = momLab2 = invbetaLab2=0;
  tkin = mom2 = invbeta2=beta=gamma=0;

  Trec=targetZ = targetMass = As =0;
  etag = ecut = 0.0;

  targetA = 0;

  cosTetMinNuc=0;
  cosTetMaxNuc=0;

  for(G4int i=0 ; i<5;  ++i){
    for(G4int j=0; j< 6; ++j){
      coeffb[i][j]=0;
    }
  }
  if(dangle[0] == 0.0) {
    for(G4int i=0; i<DIM; ++i){
      cross[i]=0;
    }
  
 
  }

  mottcoeff = new G4MottCoefficients();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4ScreeningMottCrossSection::~G4ScreeningMottCrossSection()
{
  delete mottcoeff;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void G4ScreeningMottCrossSection::Initialise(const G4ParticleDefinition* p,
                                          G4double CosThetaLim)
{
  SetupParticle(p);
  tkin = targetZ = mom2 = DBL_MIN;
  ecut = etag = DBL_MAX;
  particle = p;
  cosThetaMin = CosThetaLim; 

}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
void G4ScreeningMottCrossSection::SetScreeningCoefficient()
{
  G4double alpha2=alpha*alpha;
  //Bohr radius 
  G4double a0=  Bohr_radius  ;//0.529e-8*cm;
  //Thomas-Fermi screening length
  G4double aU=0.88534*a0/fG4pow->Z13(targetZ);
  G4double twoR2=aU*aU;

  G4double factor= 1.13 + 3.76*targetZ*targetZ*invbeta2*alpha2;
  As=0.25*(htc2)/(twoR2*mom2)*factor;
  //cout<<"0k .........................As  "<<As<<endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4double G4ScreeningMottCrossSection::GetScreeningAngle()
{
  SetScreeningCoefficient();

  G4double screenangle=2.*asin(sqrt(As));
  //	cout<<"  screenangle  "<<  screenangle <<endl;
  if(screenangle>=pi) screenangle=pi;
	
  return screenangle;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void G4ScreeningMottCrossSection::SetupKinematic(G4double ekin, G4double Z )
{
  //...Target
  G4int iz = G4int(Z);
  G4double A = fNistManager->GetAtomicMassAmu(iz);
  G4int ia = G4int(A);
  G4double mass2 = G4NucleiProperties::GetNuclearMass(ia, iz);

  targetZ = Z;
  targetA = fNistManager->GetAtomicMassAmu(iz);
  targetMass= mass2;

  mottcoeff->SetMottCoeff(targetZ, coeffb);

  //cout<<"......... targetA "<< targetA <<endl;
  //cout<<"......... targetMass "<< targetMass/MeV <<endl;

  // incident particle lab
  tkinLab = ekin;
  momLab2 = tkinLab*(tkinLab + 2.0*mass);
  invbetaLab2 = 1.0 +  mass*mass/momLab2;

  G4double etot = tkinLab + mass;
  G4double ptot = sqrt(momLab2);
  G4double m12  = mass*mass;
                
  // relativistic reduced mass from publucation
  // A.P. Martynenko, R.N. Faustov, Teoret. mat. Fiz. 64 (1985) 179
        
  //incident particle & target nucleus
  G4double Ecm=sqrt(m12 + mass2*mass2 + 2.0*etot*mass2);
  mu_rel=mass*mass2/Ecm;
  G4double momCM= ptot*mass2/Ecm;
  // relative system
  mom2 = momCM*momCM;
  invbeta2 = 1.0 +  mu_rel*mu_rel/mom2;
  tkin = momCM*sqrt(invbeta2) - mu_rel;//Ekin of mu_rel
  G4double  beta2=1./invbeta2;
  beta=std::sqrt(beta2) ;
  G4double gamma2= 1./(1.-beta2);
  gamma=std::sqrt(gamma2);

  //.........................................................

  
  SetScreeningCoefficient();

  //Integration Angles definition

  cosTetMinNuc =cosThetaMin;
  cosTetMaxNuc =cosThetaMax;

  for(G4int i=0; i<DIM;  ++i ){
    cross[i]=0;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4double G4ScreeningMottCrossSection::FormFactor2ExpHof(G4double angles)
{
  G4double M=targetMass; 
  G4double E=tkinLab;
  G4double Etot=E+mass;
  G4double Tmax=2.*M*E*(E+2.*mass)/(mass*mass+M*M+2.*M*Etot);
  G4double T=Tmax*fG4pow->powN(sin(angles*0.5), 2);
  G4double q2=T*(T+2.*M);
  q2/=htc2;//1/cm2
  G4double RN=1.27e-13*G4Exp(G4Log(targetA)*0.27)*cm;
  G4double xN= (RN*RN*q2);
  G4double den=(1.+xN/12.);
  G4double FN=1./(den*den);
  G4double form2=(FN*FN);

  return form2;

  //cout<<"..................... form2 "<< form2<<endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4double G4ScreeningMottCrossSection::McFcorrection(G4double angles )
{
  G4double  beta2=1./invbeta2;
  G4double sintmezzi=std::sin(angles/2.);
  G4double sin2tmezzi = sintmezzi*sintmezzi;
  G4double R=1.-beta2*sin2tmezzi + targetZ*alpha*beta*pi*sintmezzi*(1.-sintmezzi);
  return R;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
G4double G4ScreeningMottCrossSection::RatioMottRutherford(G4double angles)
{
  G4double R=0;
  G4double fcost=std::sqrt((1. -cos(angles)));
  G4double a[5];
  const G4double shift=0.7181228;
  G4double beta0= beta -shift;

  for(G4int j=0 ;j<=4;j++){
    a[j]=0;
  }

  for(G4int j=0 ;j<=4;j++){
    for(G4int k=0;k<=5;k++ ){  
      a[j]+=coeffb[j][k]*fG4pow->powN(beta0,k);
    }
  }

  for(G4int j=0; j<=4; ++j){
    R+=a[j]*fG4pow->powN(fcost,j);
  }
  return R;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4double G4ScreeningMottCrossSection::NuclearCrossSection()
{
  if(cosTetMaxNuc >= cosTetMinNuc) return 0.0;

  TotalCross=0;

  for(G4int i=0; i<DIM; ++i){
    G4double R=0;
    G4double F2=FormFactor2ExpHof(tet[i]);
			
    if (coeffb[0][0]!=0){
      //cout<<" Mott....targetZ "<< targetZ<<endl;	
      R=RatioMottRutherford(tet[i]);
    } else if (coeffb[0][0]==0){
      // cout<<" McF.... targetZ "<< targetZ<<endl;
      R=McFcorrection(tet[i]);
    }

    //cout<<"----------------- R "<<R<<" F2 "<<F2<<endl;
    //                cout<<"angle "<<tet[i] << " F2 "<<F2<<endl;

    G4double e4=e2*e2;
    G4double den=2.*As+2.*fG4pow->powN(sin(tet[i]*0.5),2);
    G4double func=1./(den*den);

    G4double fatt= targetZ/(mu_rel*gamma*beta*beta);
    G4double sigma=e4*fatt*fatt*func;
    cross[i]=F2*R*sigma;
    G4double pi2sintet=twopi*sin(tet[i]);

    TotalCross+=pi2sintet*cross[i]*dangle[i];
  }//end integral

  //cout<< "ok ......... TotalCross "<<TotalCross<<endl;
  return TotalCross;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4double G4ScreeningMottCrossSection::AngleDistribution(G4double anglein)
{
  G4double total=TotalCross ;
  G4double fatt= e2*targetZ/(mu_rel*gamma*beta*beta);
  G4double fatt2=fatt*fatt;
  total/=fatt2;

  G4double R=0;
  if (coeffb[0][0]!=0){
    //   cout<<" Mott....targetZ "<< targetZ<<endl;      
    R=RatioMottRutherford(anglein);
  } else if (coeffb[0][0]==0){
    // cout<<" McF.... targetZ "<< targetZ<<endl;
    R=McFcorrection(anglein);
  }

  G4double y=twopi*sin(anglein)*R*FormFactor2ExpHof(anglein)/
    ( (2*As+2.*fG4pow->powN(sin(anglein*0.5),2))*(2.*As+2.*fG4pow->powN(sin(anglein*0.5),2)) );
  //G4cout<<"AngleDistribution Total Cross: "<<TotalCross<<" y: "<<y<<G4endl;//TO REMOVE
  return y/total;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4double G4ScreeningMottCrossSection::GetScatteringAngle()
{
  //	cout<<"................ tkinLab  "<< G4BestUnit(tkinLab,"Energy") << " anglemin=  "<<anglemin<<endl;
  //cout<<"anglemax=  "<<anglemax<<endl;
  G4double r =G4UniformRand();
  
  G4double scattangle=0;
  G4double y=0;
  G4double dy=0;
  G4double area=0;

  for(G4int i=0; i<DIM; ++i){
    
    y+=AngleDistribution(tet[i])*dangle[i];
    dy= y-area ;
    area=y;

    if(r >=y-dy && r<y ){	
      scattangle= angle[i] +G4UniformRand()*dangle[i];
      //G4cout<<"Energy: "<<tkinLab/MeV<<" Random: "<<r<<" y: "<<y<<" Scattangle: "<<scattangle<<" Angle: "<<angle[i]<<" dAngle: "<<dangle[i]<<G4endl; //TO REMOVE
      break;
    }			
  }
  return scattangle;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ThreeVector G4ScreeningMottCrossSection::GetNewDirection(){

  G4ThreeVector dir(0.0,0.0,1.0);
	
  G4double z1=GetScatteringAngle();
  
  G4double sint = sin(z1);
  G4double cost = sqrt(1.0 - sint*sint);
  G4double phi  = twopi* G4UniformRand();
  G4double dirx = sint*cos(phi);
  G4double diry = sint*sin(phi);
  G4double dirz = cost;

  //.......set Trc
  G4double etot=tkinLab+mass;
  G4double mass2=targetMass;
  Trec=(1.0 - cost)* mass2*(etot*etot - mass*mass )/
    (mass*mass + mass2*mass2+ 2.*mass2*etot);
       
  dir.set(dirx,diry,dirz);

  return dir;
}

