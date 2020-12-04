#include <iostream>
#include <fstream>
#include <queue>
#include <chrono>
#include <utility>
#include <vector>
#include "tournament_sort.h"

using namespace std;
using namespace std::chrono;

class PriorityQueueTop {
public:
    string value;
    int index;

    PriorityQueueTop(string new_value, int new_idx) {
        value = move(new_value);
        index = new_idx;
    }
};

class PriorityQueueComparator {
public:
    bool operator()(const PriorityQueueTop& v1, const PriorityQueueTop& v2) {
        return v1.value > v2.value;
    }
};

void concatenate_data(int &files_count) {
    auto t1 = high_resolution_clock::now();

    vector<ifstream> input_batches(files_count);

    for (int i = 0; i < files_count; ++i) {
        string file_name = "../files/" + to_string(i) + ".txt";
        input_batches[i].open(file_name);
    }
    ofstream output;
    output.open("../output.txt");

    priority_queue<PriorityQueueTop, vector<PriorityQueueTop>, PriorityQueueComparator> priority_queue;

    basic_string<char, char_traits<char>, allocator<char>> str;
    for (int i = 0; i < files_count; ++i) {
        input_batches[i] >> str;

        PriorityQueueTop N(str, i);
        priority_queue.push(N);
    }

    while (!priority_queue.empty()) {
        PriorityQueueTop node = priority_queue.top();
        priority_queue.pop();

        output << node.value << "\n";

        if (input_batches[node.index] >> str) {
            node.value = str;
            priority_queue.push(node);
        }
    }

    for (int i = 0; i < files_count; ++i) {
        string file_name = to_string(i) + ".txt";
        input_batches[i].close();
    }
    output.close();

    auto t2 = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(t2 - t1).count();
    cout << "Merge done in " << duration << " ms\n";
}

void split_data_and_sort(ifstream &input_file, int file_size, int &file_count) {
    ofstream out_file;
    vector<string> data_to_sort;
    string input_string;

    bool not_eof = true;
    int rows_count = 0;
    while (not_eof) {
        data_to_sort.clear();
        while (rows_count < file_size) {
            if (input_file >> input_string) {
                data_to_sort.push_back(input_string);
                rows_count++;
            } else {
                not_eof = false;
                break;
            }
        }
        if (data_to_sort.empty()) break;

        auto t1 = high_resolution_clock::now();

        data_to_sort = tournament_sort(data_to_sort).sorted();

        auto t2 = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(t2 - t1).count();
        cout << file_count << " batch sorted in " << duration << " ms\n";

        string file_name = "../files/" + to_string(file_count) + ".txt";
        out_file.open(file_name);
        for (const string& str : data_to_sort)
            out_file << str << " ";

        out_file.close();
        rows_count = 0;
        file_count++;
    }
}

int main() {
    int max_file_size = 500000;
    int total_files_count = 0;

    ifstream input_file;
    input_file.open("../input.txt");

    auto t1_all_time = high_resolution_clock::now();

    split_data_and_sort(input_file, max_file_size, total_files_count);

    auto t2_time_on_split = high_resolution_clock::now();

    cout << "\nTime spend on splitting and sorting:  " << duration_cast<milliseconds>(t2_time_on_split - t1_all_time).count() << " ms\n";

    concatenate_data(total_files_count);

    auto t2_all_time = high_resolution_clock::now();

    auto duration_all = duration_cast<milliseconds>(t2_all_time - t1_all_time).count();

    cout << "\nTotal algorithm time:  " << duration_all << " ms\n";

    return 0;
}
