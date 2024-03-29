#ifndef __CHECKBOX_H__
#define __CHECKBOX_H__

#include "Widget.h"
#include "../Bitmap/Bmp.h"
#include "../Histogram/Histogram.h"
#include <string>

using namespace std;

class Checkbox : public Widget
{
public:
    int isChecked;
    string label;
    vector<float> labelColor;
    vector<Checkbox*> check;
    Bmp** imgController;
    Histogram** histController;

    Checkbox();
    Checkbox(Bmp** img, Histogram** hist);
    ~Checkbox();
    Checkbox(int x, int y, float w, float h, int isChecked, string label, vector<float> labelColor, vector<float> bg);
    void Render() override;
    void CheckState(int state, int x, int y) override;
    void Create () override;
    void RenderWidgets() override;
};

#endif

