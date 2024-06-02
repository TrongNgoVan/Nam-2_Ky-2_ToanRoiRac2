#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>
using namespace std;
// Định nghĩa cấu trúc dữ liệu lưu trữ các đặc trưng của một email
struct Email {
    string sender;
    string receiver;
    string subject;
    string body;
    vector<string> keywords;
    bool is_spam;
};

// Định nghĩa hàm đọc dữ liệu từ file vào một vector các email
vector<Email> read_data(string filename) {
    vector<Email> data;
    ifstream file(filename);
    string line;

    while (getline(file, line)) {
        Email email;
        stringstream ss(line);
        string field;

        getline(ss, field, ',');
        email.sender = field;

        getline(ss, field, ',');
        email.receiver = field;

        getline(ss, field, ',');
        email.subject = field;

        getline(ss, field, ',');
        email.body = field;

        while (getline(ss, field, ',')) {
            email.keywords.push_back(field);
        }

        getline(ss, field);
        email.is_spam = (field == "spam");

        data.push_back(email);
    }

    return data;
}

// Định nghĩa hàm tính toán sigmoid của một số thực
double sigmoid(double x) {
    return 1 / (1 + exp(-x));
}

// Định nghĩa hàm huấn luyện mô hình Logistic Regression
vector<double> train_logistic_regression(vector<vector<double>>& features, vector<double>& labels, double learning_rate, int num_iterations) {
    int num_examples = features.size();
    int num_features = features[0].size();

    // Khởi tạo các tham số của mô hình
    vector<double> weights(num_features);
    double bias = 0;

    // Huấn luyện mô hình
    for (int i = 0; i < num_iterations; i++) {
        double cost = 0;
        vector<double> dw(num_features);
        double db = 0;

        for (int j = 0; j < num_examples; j++) {
            double z = 0;
            for (int k = 0; k < num_features; k++) {
                z += weights[k] * features[j][k];
            }
            z += bias;

            double a = sigmoid(z);
            cost += -labels[j] * log(a) - (1 - labels[j]) * log(1 - a);

            for (int k = 0; k < num_features; k++) {
                dw[k] += (a - labels[j]) * features[j][k];
            }
            db += a - labels[j];
        }

        cost /= num_examples;
        for (int k = 0; k < num_features; k++) {
            weights[k] -= learning_rate * dw[k] / num_examples;
        }
        bias -= learning_rate * db / num_examples;

        cout << "Iteration " << i << " Cost " << cost << endl;
    }

    // Trả về các tham số đã học được
    vector<double> params(num_features + 1);
    params[0] = bias;
    for (int i = 0; i < num_features; i++) {
        params[i + 1] = weights[i];
    }

    return params;
}

// Định nghĩa hàm dự đoán một email có phải là spam hay không
bool predict(vector<double>& params, Email& email) {
    double z = params[0];
    int num_features = params.size() - 1;
    for (int i = 0; i < num_features; i++) {
        string keyword = "keyword_" + to_string(i);
        if (find(email.keywords.begin(), email.keywords.end(), keyword) != email.keywords.end()) {
            z += params[i + 1];
        }
    }

    return sigmoid(z) >= 0.5;
}

int main() {
    // Đọc dữ liệu từ file
    vector<Email> data = read_data("emails.csv");

    // Tạo vector các đặc trưng và vector nhãn
    vector<vector<double>> features;
    vector<double> labels;
    for (auto email : data) {
        vector<double> feature_row;
        for (int i = 0; i < 20; i++) {
            string keyword = "keyword_" + to_string(i);
            if (find(email.keywords.begin(), email.keywords.end(), keyword) != email.keywords.end()) {
                feature_row.push_back(1);
            } else {
                feature_row.push_back(0);
            }
        }
        features.push_back(feature_row);
        labels.push_back(email.is_spam ? 1 : 0);
    }

    // Huấn luyện mô hình Logistic Regression
    double learning_rate = 0.1;
    int num_iterations = 100;
    vector<double> params = train_logistic_regression(features, labels, learning_rate, num_iterations);

    // Dự đoán nhãn của các email
    int num_correct = 0;
    int num_total = data.size();
    for (auto email : data) {
        bool predicted_label = predict(params, email);
        if (predicted_label == email.is_spam) {
            num_correct++;
        }
    }

    // In ra kết quả đánh giá mô hình
    double accuracy = (double)num_correct / num_total;
    cout << "Accuracy: " << accuracy << endl;

    return 0;
}
