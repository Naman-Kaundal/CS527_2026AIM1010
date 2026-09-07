#include <algorithm>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
namespace fs = std::filesystem;


bool run(const string& command)
{
    cout << "\n$ " << command << "\n\n";

    int result = system(command.c_str());

    if (result != 0) {
        cout << "\nCommand failed.\n";
        return false;
    }

    return true;
}


vector<string> getTests(const string& folder)
{
    vector<string> tests;

    for (const auto& file : fs::directory_iterator(folder)) {
        if (file.path().extension() == ".txt")
            tests.push_back(file.path().string());
    }

    sort(tests.begin(), tests.end());

    return tests;
}


void showTests(const vector<string>& tests)
{
    for (int i = 0; i < (int)tests.size(); i++)
        cout << i + 1 << ". " << tests[i] << "\n";
}


int main()
{
    while (true)
    {
        cout << "\n========================================\n";
        cout << "          CS509 COMMON WRAPPER\n";
        cout << "========================================\n";

        cout << "\nSelect Assignment:\n";
        cout << "1. Assignment 1\n";
        cout << "2. Assignment 2\n";
        cout << "3. Assignment 3\n";
        cout << "4. Assignment 4\n";
        cout << "0. Exit\n";

        int assignment;
        cout << "\nEnter choice: ";
        cin >> assignment;

        if (assignment == 0)
            break;


        // ==========================================
        // ASSIGNMENT 1
        // ==========================================

        if (assignment == 1)
        {
            cout << "\nSelect Algorithm:\n";
            cout << "1. GEMM\n";
            cout << "2. CSR Graph\n";
            cout << "0. Back\n";

            int choice;
            cout << "\nEnter choice: ";
            cin >> choice;

            if (choice == 0)
                continue;


            string exe;
            string testFolder;
            string compileCommand;


            if (choice == 1)
            {
                exe = "gemm";
                testFolder = "assignment_01/01_GEMM/tests";

                compileCommand =
                    "g++ -std=c++17 -O2 "
                    "assignment_01/01_GEMM/src/gemm.cpp "
                    "assignment_01/01_GEMM/driver/driver_gemm.cpp "
                    "-o gemm";
            }
            else if (choice == 2)
            {
                exe = "csr";
                testFolder = "assignment_01/02_CSR_Graph/tests";

                compileCommand =
                    "g++ -std=c++17 -O2 "
                    "assignment_01/02_CSR_Graph/src/csr_graph.cpp "
                    "assignment_01/02_CSR_Graph/driver/driver_csr.cpp "
                    "-o csr";
            }
            else
            {
                cout << "\nInvalid choice.\n";
                continue;
            }


            if (!run(compileCommand))
                continue;


            vector<string> tests = getTests(testFolder);

            cout << "\nSelect Test Case:\n";
            showTests(tests);
            cout << "0. Back\n";

            int test;
            cout << "\nEnter choice: ";
            cin >> test;

            if (test == 0)
                continue;

            if (test < 1 || test > (int)tests.size())
            {
                cout << "\nInvalid test case.\n";
                continue;
            }

            run("./" + exe + " \"" + tests[test - 1] + "\"");
        }


        // ==========================================
        // ASSIGNMENT 2
        // ==========================================

        else if (assignment == 2)
        {
            cout << "\nSelect Algorithm:\n";
            cout << "1. Bellman-Ford\n";
            cout << "2. Floyd-Warshall\n";
            cout << "0. Back\n";

            int choice;
            cout << "\nEnter choice: ";
            cin >> choice;

            if (choice == 0)
                continue;


            string exe;
            string testFolder;
            string compileCommand;


            if (choice == 1)
            {
                exe = "bellman_ford";
                testFolder = "assignment_02/01_Bellman_Ford/tests";

                compileCommand =
                    "g++ -std=c++17 -O2 "
                    "assignment_01/02_CSR_Graph/src/csr_graph.cpp "
                    "assignment_02/01_Bellman_Ford/src/bellman_ford.cpp "
                    "assignment_02/01_Bellman_Ford/driver/driver_bellman_ford.cpp "
                    "-o bellman_ford";
            }
            else if (choice == 2)
            {
                exe = "floyd_warshall";
                testFolder = "assignment_02/02_Floyd_Warshall/tests";

                compileCommand =
                    "g++ -std=c++17 -O2 "
                    "assignment_02/02_Floyd_Warshall/src/floyd_warshall.cpp "
                    "assignment_02/02_Floyd_Warshall/driver/driver_floyd_warshall.cpp "
                    "-o floyd_warshall";
            }
            else
            {
                cout << "\nInvalid choice.\n";
                continue;
            }


            if (!run(compileCommand))
                continue;


            vector<string> tests = getTests(testFolder);

            cout << "\nSelect Test Case:\n";
            showTests(tests);
            cout << "0. Back\n";

            int test;
            cout << "\nEnter choice: ";
            cin >> test;

            if (test == 0)
                continue;

            if (test < 1 || test > (int)tests.size())
            {
                cout << "\nInvalid test case.\n";
                continue;
            }

            run("./" + exe + " \"" + tests[test - 1] + "\"");
        }


        // ==========================================
        // ASSIGNMENT 3
        // ==========================================

        else if (assignment == 3)
        {
            cout << "\nSelect Algorithm:\n";
            cout << "1. Kruskal\n";
            cout << "2. Prim\n";
            cout << "0. Back\n";

            int choice;
            cout << "\nEnter choice: ";
            cin >> choice;

            if (choice == 0)
                continue;


            string exe;
            string testFolder;
            string compileCommand;


            if (choice == 1)
            {
                exe = "kruskal";
                testFolder = "assignment_03/tests";

                compileCommand =
                    "g++ -std=c++17 -O2 "
                    "assignment_01/02_CSR_Graph/src/csr_graph.cpp "
                    "assignment_03/01_Kruskal/src/kruskal.cpp "
                    "assignment_03/01_Kruskal/driver/driver_kruskal.cpp "
                    "-o kruskal";
            }
            else if (choice == 2)
            {
                exe = "prim";
                testFolder = "assignment_03/tests";

                compileCommand =
                    "g++ -std=c++17 -O2 "
                    "assignment_01/02_CSR_Graph/src/csr_graph.cpp "
                    "assignment_03/02_Prim/src/prim.cpp "
                    "assignment_03/02_Prim/driver/driver_prim.cpp "
                    "-o prim";
            }
            else
            {
                cout << "\nInvalid choice.\n";
                continue;
            }


            if (!run(compileCommand))
                continue;


            vector<string> tests = getTests(testFolder);

            cout << "\nSelect Test Case:\n";
            showTests(tests);
            cout << "0. Back\n";

            int test;
            cout << "\nEnter choice: ";
            cin >> test;

            if (test == 0)
                continue;

            if (test < 1 || test > (int)tests.size())
            {
                cout << "\nInvalid test case.\n";
                continue;
            }

            run("./" + exe + " \"" + tests[test - 1] + "\"");
        }


        // ==========================================
        // ASSIGNMENT 4
        // ==========================================

        else if (assignment == 4)
        {
            cout << "\nSelect Algorithm:\n";
            cout << "1. Vertex Coloring\n";
            cout << "2. PageRank\n";
            cout << "0. Back\n";

            int choice;
            cout << "\nEnter choice: ";
            cin >> choice;

            if (choice == 0)
                continue;


            string exe;
            string testFolder;
            string compileCommand;


            if (choice == 1)
            {
                exe = "graph_coloring";
                testFolder = "assignment_04/01_Graph_Coloring/tests";

                compileCommand =
                    "g++ -std=c++17 -O2 "
                    "assignment_01/02_CSR_Graph/src/csr_graph.cpp "
                    "assignment_04/01_Graph_Coloring/src/graph_coloring.cpp "
                    "assignment_04/01_Graph_Coloring/driver/driver_graph_coloring.cpp "
                    "-o graph_coloring";
            }
            else if (choice == 2)
            {
                exe = "pagerank";
                testFolder = "assignment_04/02_PageRank/tests";

                compileCommand =
                    "g++ -std=c++17 -O2 "
                    "assignment_01/02_CSR_Graph/src/csr_graph.cpp "
                    "assignment_04/02_PageRank/src/pagerank.cpp "
                    "assignment_04/02_PageRank/driver/driver_pagerank.cpp "
                    "-o pagerank";
            }
            else
            {
                cout << "\nInvalid choice.\n";
                continue;
            }


            if (!run(compileCommand))
                continue;


            vector<string> tests = getTests(testFolder);

            cout << "\nSelect Test Case:\n";
            showTests(tests);
            cout << "0. Back\n";

            int test;
            cout << "\nEnter choice: ";
            cin >> test;

            if (test == 0)
                continue;

            if (test < 1 || test > (int)tests.size())
            {
                cout << "\nInvalid test case.\n";
                continue;
            }

            run("./" + exe + " \"" + tests[test - 1] + "\"");
        }


        else
        {
            cout << "\nInvalid assignment choice.\n";
        }
    }


    cout << "\nWrapper closed.\n";

    return 0;
}
