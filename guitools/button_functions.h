void doAdvanceSpill();
void doPreviousSpill();
void doUpdateStatusBar();

void doColorbySlice();
void doColorbyPFP();

void doUseSliceCuts(bool use, std::vector<int> cut_indices);
void doUseSpillCuts(bool use, std::vector<int> cut_indices);
void doUseNuSlice(bool use);
void doDrawPlane1(bool use);
void doDrawPlane2(bool use);
void doDrawPlane3(bool use);
void doDrawCRTHits(bool use);

int runstring;
int eventstring;
