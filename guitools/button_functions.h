void doAdvanceSpill();
void doPreviousSpill();
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

int runstring;
int eventstring;
