#include "NdpEncoderOptimizer.h"

namespace vvenc {

static int PREF_FRAC_GLOBAL = 12;

void NdpEncoderOptimizer::traceCtuCodingInfo(const CodingStructure& cs, const UnitArea& ctuArea) {
    DTRACE( g_trace_ctx, D_NDP_TRACE, "POC. %3d CTU: @(%4d,%4d) [%2dx%2d]\n",
        cs.slice->poc,
        ctuArea.Y().x, ctuArea.Y().y,
        ctuArea.Y().width, ctuArea.Y().height
    );
    
    const ChannelType chType = ChannelType( ChannelType::CH_L );
    for( const CodingUnit &cu : cs.traverseCUs( CS::getArea( cs, ctuArea, chType, TREE_D ), chType ) ) {
        if (cu.predMode == MODE_INTER) { 

            DTRACE( g_trace_ctx, D_NDP_TRACE, "CU: (%4d,%4d) [%2dx%2d]\n",
                cu.lx(),  
                cu.ly(),
                cu.lwidth(),
                cu.lheight()
            );
            const int nShift = MV_FRACTIONAL_BITS_DIFF;
            const int nOffset = 1 << (nShift - 1);

            if(cu.affine) {
                DTRACE( g_trace_ctx, D_NDP_TRACE, "Affine!\n");
            }
            else if(cu.geo) {
                DTRACE( g_trace_ctx, D_NDP_TRACE, "Geo!\n");
            }
            else {
                if(cu.interDir != 2) { // PRED_L0
                    int8_t refFramePoc = cu.slice->getRefPic( REF_PIC_LIST_0, cu.refIdx[REF_PIC_LIST_0] )->getPOC();
                    Mv mvOrig = cu.mv[REF_PIC_LIST_0][0];
                    Mv mv;
                    mv.hor = mvOrig.hor >= 0 ? (mvOrig.hor + nOffset) >> nShift : -((-mvOrig.hor + nOffset) >> nShift);
                    mv.ver = mvOrig.ver >= 0 ? (mvOrig.ver + nOffset) >> nShift : -((-mvOrig.ver + nOffset) >> nShift);
                    DTRACE( g_trace_ctx, D_NDP_TRACE, "L0: Ref. %2d (%3d, %3d) (%3d, %3d) [%2d]\n",
                        refFramePoc,
                        mvOrig.hor,
                        mvOrig.ver,
                        mv.hor,
                        mv.ver,
                        xGetFracPosition(mv)
                    );
                }
                if (cu.interDir != 1) { // PRED_L1
                    int8_t refFramePoc = cu.slice->getRefPic( REF_PIC_LIST_1, cu.refIdx[REF_PIC_LIST_1] )->getPOC();
                    Mv mvOrig = cu.mv[REF_PIC_LIST_1][0];
                    Mv mv;
                    mv.hor = mvOrig.hor >= 0 ? (mvOrig.hor + nOffset) >> nShift : -((-mvOrig.hor + nOffset) >> nShift);
                    mv.ver = mvOrig.ver >= 0 ? (mvOrig.ver + nOffset) >> nShift : -((-mvOrig.ver + nOffset) >> nShift);
                    DTRACE( g_trace_ctx, D_NDP_TRACE, "L1: Ref. %2d (%3d, %3d) (%3d, %3d) [%2d]\n",
                        refFramePoc,
                        mvOrig.hor,
                        mvOrig.ver,
                        mv.hor,
                        mv.ver,
                        xGetFracPosition(mv)
                    );
                }
            }            
        }      
    }
}

int NdpEncoderOptimizer::xGetFracPosition(Mv shiftMv) {
    int xQuarterMV = shiftMv.hor & 0x3;
    int yQuarterMV = shiftMv.ver & 0x3;

    int fracPosition = (xQuarterMV << 2) | yQuarterMV;

    return fracPosition;
}

}