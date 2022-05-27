#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <algorithm>

struct Inspector {
    int position;
    int speed;
};

template<typename Type>
void MergeSort(Type* buf, size_t l, size_t r, bool (*comp)(Type l, Type r));
template<typename Type>
static void merge(Type* buf, size_t l, size_t r, size_t m, bool (*comp)(Type l, Type r));

template<typename Type>
void MergeSort(std::vector<Type>& buf, size_t l, size_t r, bool (*comp)(Type l, Type r))
{

    if (l >= r) return;

    size_t m = (l + r) / 2;


    MergeSort(buf, l, m, comp);
    MergeSort(buf, m + 1, r, comp);
    merge(buf, l, r, m, comp);
}

template<typename Type>
static void merge(std::vector<Type>& buf, size_t l, size_t r, size_t m, bool (*comp)(Type l, Type r))
{
    if (l >= r || m < l || m > r) return;
    if (r == l + 1 && comp(buf[r], buf[l])) { //buf[l] > buf[r]
        std::swap(buf[l], buf[r]);
        return;
    }

    std::vector<Type> tmp(&buf[l], &buf[r] + 1);

    for (size_t i = l, j = 0, k = m - l + 1; i <= r; ++i) {
        if (j > m - l) {
            buf[i] = tmp[k++];
        }
        else if (k > r - l) {
            buf[i] = tmp[j++];
        }
        else {
            buf[i] = (comp(tmp[j], tmp[k])) ? tmp[j++] : tmp[k++]; //tmp[j] < tmp[k]
        }
    }
}

bool Comp(Inspector l, Inspector r) {
    if (std::abs(l.speed) > std::abs(r.speed)) {
        return true;
    }

    return false;
}

void activateInspectors(std::vector<double>& safeTime, std::vector<Inspector> inspectors) {
    for (int i = 0; i < inspectors.size(); i++) {
        int time = 0;
        int direction = (inspectors[i].speed < 0) ? -1 : 1;
        double needTimeForOneVagon = (double)((double)1 / std::abs(inspectors[i].speed));;

        for (int pos = inspectors[i].position; pos > 0 && pos < safeTime.size(); pos += direction) {
            double temp = time * needTimeForOneVagon;
            if (temp > safeTime[pos]) {
                break;
            }
            safeTime[pos] = temp;
            time++;
        }
    }
}

void createMas(std::vector<double>& safeTime) {
    safeTime[0] = 0;
    for (int i = 1; i < safeTime.size(); i++) {
        safeTime[i] = INT_MAX;
    }
}

int main()
{
    std::ifstream inp("input.txt");
    std::ofstream out("output.txt");

    int n, m;
    inp >> n >> m;

    std::vector<double> safeTime1(n+1);
    std::vector<double> safeTime2(n + 1);
    std::vector<Inspector> inspectorsPositiv;
    std::vector<Inspector> inspectorsNegativ;

    createMas(safeTime1);
    createMas(safeTime2);
    

    for (int i = 0; i < m; i++) {
        int tp, ts;
        inp >> tp >> ts;
        if (ts > 0) {
            inspectorsPositiv.push_back({ tp, ts });
        }
        else {
            inspectorsNegativ.push_back({ tp, ts });
        }
    }

    if (inspectorsPositiv.size() > 0) {
        MergeSort(inspectorsPositiv, 0, inspectorsPositiv.size() - 1, Comp);
        activateInspectors(safeTime1, inspectorsPositiv);
    }
    if (inspectorsNegativ.size() > 0) {
        MergeSort(inspectorsNegativ, 0, inspectorsNegativ.size() - 1, Comp);
        activateInspectors(safeTime2, inspectorsNegativ);
    }
   

    for (int i = 1; i < safeTime1.size(); i++) {
        safeTime1[i] = std::min(safeTime1[i], safeTime2[i]);
    }

    int answer = 1;
    for (int i = 2; i < safeTime1.size(); i++) {
        if (safeTime1[i] > safeTime1[answer]) {
            answer = i;
        }
    }

    out << answer;
}

