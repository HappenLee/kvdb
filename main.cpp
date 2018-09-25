#include <string>
#include <iostream>

#include "kvdb.h"
using namespace std;

int main() {

    auto ldb = KVDB::get_instance();
    cout << ldb->get("1") << endl;
    cout << ldb->get("2") << endl;
    cout << ldb->get("3") << endl;
    ldb->set("happen", "1");
    ldb->save();
}