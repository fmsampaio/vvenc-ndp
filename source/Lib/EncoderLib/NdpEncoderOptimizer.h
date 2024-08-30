#include "CommonLib/CodingStructure.h"
#include "CommonLib/UnitTools.h"
#include "CommonLib/dtrace_next.h"

namespace vvenc {

class NdpEncoderOptimizer {
    private:

    
    public:
        static void traceCtuCodingInfo(const CodingStructure& cs, const UnitArea& ctuArea);
        static int xGetFracPosition(Mv shiftMv);

        static int getPrefFrac();

        static int getHalfPosition(int fracPos);
        static int getFracPosition(Mv mv, MvPrecision precision);
        
        static std::string mvToStr(Mv mv);

        static Mv shiftMvFromInternalToQuarter(Mv &origMv);
};

}