/*F***************************************************************************
 * This file is part of openSMILE.
 * 
 * Copyright (c) audEERING GmbH. All rights reserved.
 * See the file COPYING for details on license terms.
 ***************************************************************************E*/


/*  openSMILE component: cPlp

This component computes PLP and RASTA-PLP cepstral coefficients from a critical band spectrum (generated by the cMelspec component, for example).

The component is capable of performing the following processing steps:
   1) Take the natural logarithm of the critical band powers (doLog)
   2) RASTA filtering
   3) Computation of auditory spectrum (equal loudness curve and loudness compression)
   4) Inverse of the natural logarithm
   5) Inverse DFT to obtain autocorrelation coefficients
   6) Linear prediction analysis on autocorr. coeff.
   7) Computation of cepstral coefficients from lp coefficients
   8) Cepstral 'liftering'

*/



#ifndef __CPLP_HPP
#define __CPLP_HPP

#include <core/smileCommon.hpp>
#include <core/vectorProcessor.hpp>
#include <math.h>

#define COMPONENT_DESCRIPTION_CPLP "This component computes PLP and RASTA-PLP (currently the RASTA filter is not yet implemented) cepstral coefficients from a critical band spectrum (generated by the cMelspec component, for example).\n   The component is capable of performing the following processing steps: \n   1) Take the natural logarithm of the critical band powers (doLog)\n   2) RASTA filtering\n   3) Computation of auditory spectrum (equal loudness curve and loudness compression)\n   4) Inverse of the natural logarithm\n   5) Inverse DFT to obtain autocorrelation coefficients\n   6) Linear prediction analysis on autocorr. coeff.\n   7) Computation of cepstral coefficients from lp coefficients\n   8) Cepstral 'liftering'";

#define COMPONENT_NAME_CPLP "cPlp"

class cPlp : public cVectorProcessor {
  private:
    int htkcompatible;
    int nAuto, nFreq, nScale;
    int lpOrder;
    int nCeps, firstCC, lastCC;

    int doLog, doAud, doInvLog, doIDFT, RASTA, newRASTA, doLP, doLpToCeps;
    FLOAT_DMEM rastaUpperCutoff, rastaLowerCutoff;

    FLOAT_DMEM rasta_iir;
    FLOAT_DMEM rasta_fir[5];
    // rasta filter history:
    FLOAT_DMEM **rasta_buf_iir; /* size: n bands */
    FLOAT_DMEM **rasta_buf_fir; /* size: n bands * 5 (nCoeff) */
    int rasta_buf_fir_ptr; // buffer pointer (cyclic shifting)
    int rasta_init;

    FLOAT_DMEM cepLifter;
    
    FLOAT_DMEM **acf;
    FLOAT_DMEM **lpc;
    FLOAT_DMEM **ceps;

    FLOAT_DMEM compression;
    FLOAT_DMEM **eqlCurve; // equal loudness curve
    FLOAT_DMEM **costable; // IDFT
    FLOAT_DMEM **sintable; // cepstral liftering

    FLOAT_DMEM melfloor;

    int initTables( long blocksize, int idxc, int fidx );
    
  protected:
    SMILECOMPONENT_STATIC_DECL_PR

    virtual void myFetchConfig() override;
    virtual int dataProcessorCustomFinalise() override;
    virtual int setupNamesForField(int i, const char*name, long nEl) override;

    virtual int processVector(const FLOAT_DMEM *src, FLOAT_DMEM *dst, long Nsrc, long Ndst, int idxi) override;

  public:
    SMILECOMPONENT_STATIC_DECL
    
    cPlp(const char *_name);

    virtual ~cPlp();
};




#endif // __CPLP_HPP
