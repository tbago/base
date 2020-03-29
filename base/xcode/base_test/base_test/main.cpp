//
//  main.cpp
//  base_test
//
//  Created by tbago on 2019/9/16.
//  Copyright © 2019 tbago. All rights reserved.
//

#include <iostream>
#include <base/logging.h>
#include <base/file_util.h>

int main(int argc, const char * argv[]) {
    // insert code here...
    std::string test_path = "/usr/local/bin";
    file_util::EnsureEndsWithSeparator(test_path);
    std::cout<<test_path<<std::endl;

    return 0;
}
