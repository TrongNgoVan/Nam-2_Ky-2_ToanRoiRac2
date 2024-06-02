#include<bits/stdc++.h>
#include <vector>
#include <algorithm>
#include <ctime>

using namespace std;
int best_tardiness;
// Khai báo struct lưu trữ thông tin công việc
struct Job {
    int id;     // Mã công việc
    int time;   // Thời gian thực hiện công việc
    int due;    // Thời gian hạn chót
    int weight; // Trọng số
};

// Hàm tính tardiness cho một lịch làm việc
int calculate_tardiness(vector<int>& schedule, vector<Job>& jobs) {
    int tardiness = 0;
    int current_time = 0;
    for (int i = 0; i < schedule.size(); i++) {
        int job_id = schedule[i];
        Job job = jobs[job_id-1];
        current_time += job.time;
        tardiness += max(0, current_time - job.due) * job.weight;
    }
    return tardiness;
}

// Hàm thực hiện thuật toán Tuba Search
vector<int> tuba_search(vector<Job>& jobs) {
    // Khởi tạo lịch làm việc ban đầu là một hoán vị ngẫu nhiên của các công việc
    vector<int> schedule(jobs.size());
    for (int i = 0; i < jobs.size(); i++) {
        schedule[i] = i+1;
    }
    random_shuffle(schedule.begin(), schedule.end());

    // Thiết lập các tham số cho thuật toán
    int max_iterations = 1000;
    int max_consecutive_iterations = 100;
    int max_temperature = 100;
    int min_temperature = 1;
    double alpha = 0.9;

    // Tìm kiếm lịch làm việc tối ưu
    int current_tardiness = calculate_tardiness(schedule, jobs);
     best_tardiness = current_tardiness;
    vector<int> best_schedule = schedule;
    int consecutive_iterations = 0;
    double temperature = max_temperature;
    while (consecutive_iterations < max_consecutive_iterations && temperature >= min_temperature) {
        for (int i = 0; i < max_iterations; i++) {
            // Tạo một lịch làm việc mới bằng cách hoán vị hai công việc ngẫu nhiên
            int index1 = rand() % jobs.size();
            int index2 = rand() % jobs.size();
            swap(schedule[index1], schedule[index2]);

            // Tính toán tardiness cho lịch làm việc mới
            int new_tardiness = calculate_tardiness(schedule, jobs);

            // Nếu lịch mới tốt hơn lịch hiện tại, chấp nhận lịch mới
            if (new_tardiness <= current_tardiness) {
                current_tardiness = new_tardiness;
                if (new_tardiness < best_tardiness) {
                    best_tardiness = new_tardiness;
                    best_schedule = schedule;
                }
                consecutive_iterations = 0;
            }

// Nếu lịch mới không tốt hơn lịch hiện tại, có thể chấp nhận nó với xác suất P dựa trên nhiệt độ và sự khác biệt về tardiness
else {
	int delta_tardiness = new_tardiness - current_tardiness;
	double acceptance_probability = exp(-delta_tardiness / temperature);
	double random_number = (double)rand() / RAND_MAX;
	if (random_number < acceptance_probability) {
		current_tardiness = new_tardiness;
		consecutive_iterations = 0;
}
else {
// Nếu không chấp nhận lịch mới, phục hồi lịch cũ bằng cách hoán vị lại hai công việc
swap(schedule[index1], schedule[index2]);
consecutive_iterations++;
}
}

// Thêm lịch hiện tại vào danh sách tabu
        // Danh sách tabu lưu trữ các hoán vị gần đây, được sử dụng để tránh các hoán vị trùng lặp
        // Danh sách tabu sẽ được xóa sau khi đạt được một số điều kiện
        // Danh sách tabu được lưu dưới dạng một vector chứa các cặp (index1, index2) biểu thị cho hoán vị đã được thực hiện
        vector<pair<int, int>> tabu_list;
        tabu_list.push_back(make_pair(index1, index2));

        // Xóa các hoán vị đã lưu trong danh sách tabu nếu chúng đã tồn tại trong danh sách tabu quá lâu
        // Điều này giúp tránh việc lặp lại các hoán vị giống nhau
        for (int j = tabu_list.size() - 1; j >= 0; j--) {
            tabu_list[j].second--;
            if (tabu_list[j].second == 0) {
                tabu_list.erase(tabu_list.begin() + j);
            }
        }
    }
    temperature *= alpha;
}
return best_schedule;
}
int main() {
// Khởi tạo một số công việc để làm thí nghiệm
int n;
cin >> n;
vector<Job> jobs(n);
for(int i=0;i<n;i++) {
	cin>>jobs[i].id >> jobs[i]. time >> jobs[i].due >> jobs[i].weight;
}
//4
//1 10 4 14
//2 10 2 12
//3 13 1 1
//4 4 12 12
vector<int> best_schedule = tuba_search(jobs);

// In ra lịch làm việc tối ưu
cout << "Best schedule: ";
for (int i = 0; i < best_schedule.size(); i++) {
    cout << best_schedule[i] << " ";
}
cout << endl;
cout<<best_tardiness;
}