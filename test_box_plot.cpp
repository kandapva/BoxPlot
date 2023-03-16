#include <iostream>
#include <vector>
#include "boxPlot.hpp"

int main(){
    //std::vector<int> vec{57, 57, 57, 58, 63, 66, 66, 67, 67, 68, 69, 70, 70, 70, 70, 72, 73, 75, 75, 76, 76, 78, 79, 81};
    std::vector<std::vector<int> > vec(3);
    int a[] = {52, 57, 57, 58, 63, 66, 66, 67, 67, 68, 69, 70, 70, 70, 70, 72, 73, 75, 75, 76, 76, 78, 79, 89};
    int b[] = {57, 57, 57, 58, 63, 66, 66, 67, 67, 68, 69, 70, 70, 70, 70, 72, 73, 75, 75, 76, 76, 78, 79, 81};
    int c[] = { 136, 140, 178, 190,205,215,217,
    218,232,234,240,255,270,275,
    290,301,303,315,317,318,326,
    333,343,349,360,369,377,388,
    391,392,398,400,402,405,408,
    422,429,450,475,512
};
    for(int j=0;j<24;j++)
        vec[0].push_back(a[j]);
    for (int j = 0; j < 24; j++)
        vec[1].push_back(b[j]);
    for (int j = 0; j < 40; j++)
        vec[2].push_back(c[j]);
    std::vector<std::string> sname{"Temp 1", "Temp 2", "Pages"};
    boxPlot<int> bp1(vec, sname);
    bp1.generate_latex_file();
    boxPlot<int> bp2(vec);
    bp2.generate_latex_file();

    return 0;
}