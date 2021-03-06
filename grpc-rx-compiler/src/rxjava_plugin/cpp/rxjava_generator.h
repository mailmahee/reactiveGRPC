#ifndef NET_GRPC_COMPILER_RXJAVA_GENERATOR_H_
#define NET_GRPC_COMPILER_RXJAVA_GENERATOR_H_

#include <stdlib.h>  // for abort()
#include <iostream>
#include <string>

#include <google/protobuf/io/zero_copy_stream.h>
#include <google/protobuf/descriptor.h>

class LogHelper {
  std::ostream* os;
  bool abort;

 public:
  LogHelper(std::ostream* os, bool abort) : os(os), abort(abort) {}
  ~LogHelper() {
    *os << std::endl;
    if (abort) {
      ::abort();
    }
  }
  std::ostream& get_os() {
    return *os;
  }
};

// Abort the program after logging the mesage if the given condition is not
// true. Otherwise, do nothing.
#define GRPC_CODEGEN_CHECK(x) !(x) && LogHelper(&std::cerr, true).get_os() \
                             << "CHECK FAILED: " << __FILE__ << ":" \
                             << __LINE__ << ": "

// Abort the program after logging the mesage.
#define GRPC_CODEGEN_FAIL GRPC_CODEGEN_CHECK(false)

using namespace std;

namespace rxjava_grpc_generator {

enum ProtoFlavor {
  NORMAL, LITE, NANO
};

// Returns the package name of the gRPC services defined in the given file.
string ServiceJavaPackage(const google::protobuf::FileDescriptor* file, bool nano);

// Returns the name of the outer class that wraps in all the generated code for
// the given service.
string ServiceClassName(const google::protobuf::ServiceDescriptor* service);

// Writes the generated service interface into the given ZeroCopyOutputStream
void GenerateService(const google::protobuf::ServiceDescriptor* service,
                     google::protobuf::io::ZeroCopyOutputStream* out,
                     ProtoFlavor flavor,
                     bool enable_deprecated);

}  // namespace rxjava_grpc_generator

#endif  // NET_GRPC_COMPILER_RXJAVA_GENERATOR_H_
