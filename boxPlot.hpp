#ifndef __boxPlot__
#define __boxPlot__

#include <vector>
#include <algorithm>
#include <fstream>

template <typename T>
bool isEven(T num){
    if(num % 2 == 0)
        return true;
    else
        return false;
}

// This header helps in creating the boxplot
// Produces a latex code that is ready to use
// Data Vector of vectors
template <typename T>
class boxPlot
{
    std::vector<std::vector<T> > vec;
    std::vector<double> Q0, Q1, Q2, Q3, Q4, IQR;
    std::vector<std::string> series_name;
    size_t num_series;
    // Q0 -> min value
    // Q1 -> lower quartile qn(0.25), it is the median of the lower half of the dataset.
    // Q2 -> Median of the dataset
    // Q3 -> upper quartile qn(0.25), it is the median of the upper half of the dataset.
    // IQR -> Inter Quartile Range
    std::vector<double> whisker_lower, whisker_upper;
    void print_latex_series(int series_num, std::ofstream& outfile)
    {
        outfile << "\\addplot+[boxplot prepared={" << std::endl;
        outfile << "median=" << Q2[series_num] << "," << std::endl;
        outfile << "upper quartile=" << Q1[series_num] << "," << std::endl;
        outfile << "lower quartile=" << Q3[series_num] << "," << std::endl;
        outfile << "upper whisker=" << whisker_upper[series_num] << "," << std::endl;
        outfile << "lower whisker=" << whisker_lower[series_num] << "}," << std::endl;
        outfile << "] coordinates {};" << std::endl;
    }
    void print_details(){
    for(int i=0;i<num_series;i++){
       std::cout << "Max            - " << Q4[i] << std::endl;
       std::cout << "Upper whisker  - " << whisker_upper[i] << std::endl;
       std::cout << "Upper Quartlie - " << Q3[i] << std::endl;
       std::cout << "Median         - " << Q2[i] << std::endl;
       std::cout << "Lower Quartlie - " << Q1[i] << std::endl;
       std::cout << "Lower whisker  - " << whisker_lower[i] << std::endl;
       std::cout << "Min            - " << Q0[i] << std::endl;
    }
    }
    void compute_quartiles(int series_num)
    {
       size_t N = vec[series_num].size();
       std::sort(vec[series_num].begin(), vec[series_num].end(), std::less<T>());
       Q0[series_num] = (double)vec[series_num].at(0);
       Q4[series_num] = (double)vec[series_num].at(N - 1);
       size_t mid_id = N / 2;
       size_t upper_id, lower_id;
       // Finding Q2
       if (isEven<size_t>(N))
       {
           upper_id = N - mid_id;
           lower_id = mid_id;
           Q2[series_num] = 0.5 * double(vec[series_num].at(mid_id) + vec[series_num].at(mid_id - 1));
           if (isEven<size_t>(upper_id))
           {
               upper_id /= 2;
               Q3[series_num] = 0.5 * double(vec[series_num].at(mid_id + upper_id) + vec[series_num].at(mid_id + upper_id - 1));
           }
           else
           {
               Q3[series_num] = (double)vec[series_num].at(mid_id + upper_id);
           }
           if (isEven<size_t>(lower_id))
           {
               lower_id /= 2;
               Q1[series_num] = 0.5 * double(vec[series_num].at(lower_id) + vec[series_num].at(lower_id - 1));
           }
           else
           {
               Q1[series_num] = (double)vec[series_num].at(lower_id);
           }
       }
       else
       {
           upper_id = mid_id;
           lower_id = N - 1 - mid_id;
           Q2[series_num] = (double)vec[series_num].at(mid_id);
           if (isEven<size_t>(upper_id))
           {
               upper_id /= 2;
               Q3[series_num] = 0.5 * double(vec[series_num].at(mid_id + upper_id) + vec[series_num].at(mid_id + upper_id - 1));
           }
           else
           {
               Q3[series_num] = (double)vec[series_num].at(mid_id + upper_id);
           }
           if (isEven<size_t>(lower_id))
           {
               lower_id /= 2;
               Q1[series_num] = 0.5 * double(vec[series_num].at(lower_id) + vec[series_num].at(lower_id - 1));
           }
           else
           {
               Q1[series_num] = (double)vec[series_num].at(lower_id);
           }
       }

       // IQR
       IQR[series_num] = Q3[series_num] - Q1[series_num];
       whisker_lower[series_num] = Q1[series_num] - 1.5 * IQR[series_num];
       whisker_upper[series_num] = Q3[series_num] + 1.5 * IQR[series_num];
       if (whisker_lower[series_num] < Q0[series_num])
           whisker_lower[series_num] = Q0[series_num];
       else
       {
           int i = 0;
           while (vec[series_num].at(i) < whisker_lower[series_num])
           {
               if (++i == N)
                   break;
           }
           whisker_lower[series_num] = (double)vec[series_num].at(i);
       }
       if (whisker_upper[series_num] > Q4[series_num])
           whisker_upper[series_num] = (double)Q4[series_num];
       else
       {
           int i = N - 1;
           while (vec[series_num].at(i) > whisker_upper[series_num])
           {
               if (--i == 0)
                   break;
           }
           whisker_upper[series_num] = (double)vec[series_num].at(i);
       }
    }
    void print_header(std::ofstream& outfile)
    {
       outfile << "\\documentclass{article}" << std::endl;
       outfile << "\\usepackage {pgfplots}" << std::endl;
       outfile << "\\pgfplotsset{compat=1.8}" << std::endl;
       outfile << "\\usepgfplotslibrary{statistics}" << std::endl;
       outfile << "\\begin{document}" << std::endl;
       outfile << "\\begin{tikzpicture}" << std::endl;
       outfile << "\\begin{axis}";
       int iter = 0;
       outfile << "[ytick = {" << ++iter;
       while(iter<num_series)
           outfile << "," << ++iter;
       iter = 0;
       outfile << "}";
       outfile << ",yticklabels = {" << series_name[iter++];
       while (iter < num_series){
           outfile << ", " << series_name[iter++];
       }          
       outfile << "},]" << std::endl;
    }

public: 
    boxPlot() {}
    boxPlot(std::vector<std::vector<T>> vec_, std::vector<std::string> series_name_)
    {
        vec = vec_;
        num_series = vec.size();
        for(int i=0;i<num_series;i++){
            Q0.push_back(0.0);
            Q1.push_back(0.0);
            Q2.push_back(0.0);
            Q3.push_back(0.0);
            Q4.push_back(0.0);
            IQR.push_back(0.0);
            whisker_lower.push_back(0.0);
            whisker_upper.push_back(0.0);
        }
        series_name = series_name_;
    }
    boxPlot(std::vector<std::vector<T>> vec_)
    {
        vec = vec_;
        num_series = vec.size();
        for (int i = 0; i < num_series; i++)
        {
            Q0.push_back(0.0);
            Q1.push_back(0.0);
            Q2.push_back(0.0);
            Q3.push_back(0.0);
            Q4.push_back(0.0);
            IQR.push_back(0.0);
            whisker_lower.push_back(0.0);
            whisker_upper.push_back(0.0);
            std::string tmp = "Series " + std::to_string(i+1);
            series_name.push_back(tmp);
        }
        
    }
   void generate_latex_file(){
        std::ofstream outfile;
        outfile.open("boxPlot.tex");
        print_header(outfile);
        for(int i=0; i<num_series; i++){
            compute_quartiles(i);
            print_latex_series(i, outfile);
        }
        outfile << "\\end{axis}" << std::endl;
        outfile << "\\end{tikzpicture}" << std::endl;
        outfile << "\\end{document}" << std::endl;
        outfile.close();
        //print_details();
    }

    ~boxPlot(){}
};

#endif