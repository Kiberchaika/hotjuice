#import <Foundation/Foundation.h>
#include <string>

std::string _getAppDataDirectory() {
    NSFileManager *fm = [NSFileManager new];
    NSURL *url = [fm URLForDirectory:NSApplicationSupportDirectory
                            inDomain:NSUserDomainMask
                   appropriateForURL:nil
                              create:YES
                               error:NULL];
    char path_c[PATH_MAX];
    [url getFileSystemRepresentation:path_c maxLength:sizeof(path_c)];
    return std::string(path_c) + "/";
}
