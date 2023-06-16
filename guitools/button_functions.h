void doAdvanceSpill(float minTime, float maxTime);
void doPreviousSpill(float minTime, float maxTime);
void doUpdateStatusBar();

void doColorbySlice();
void doColorbyPFP();

void doUseSliceCuts(std::vector<int> cut_indices);
void doUseSpillCuts(std::vector<int> cut_indices);
void doUseNuSlice(bool use);
void doDrawPlane1(bool use);
void doDrawPlane2(bool use);
void doDrawPlane3(bool use);
void doTimeSel(float minTime, float maxTime);
void doDrawCRTHits(bool use, float minTime, float maxTime);
void doDrawFlashes(bool use, float minTime, float maxTime);
void doDrawReco(bool use);

int runstring;
int eventstring;
