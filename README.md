# MyPoly1305

This is a C implementation of the "Poly1305" authenticator using [GMP](https://gmplib.org/) and following [RFC 8439](https://datatracker.ietf.org/doc/html/rfc8439).

## Initialization

Install [GMP](https://gmplib.org/) on your computer.

For Debian, Ubuntu or Fedora users :
```bash
sudo apt install libgmp-dev # For Debian/Ubuntu based distribution
sudo dnf install gmp # For Fedora/RedHat based distribution
```

Use make in the root directory to build the project.
```bash
make
```

Alternatively, you can target specific program to be built:
```bash
make gen # To build tag generation
make check # To build tag checking
```

## Usage

To get a tag:
```bash
./poly1305-gen key filename
```

For example:
```bash
./poly1305-gen 85d6be7857556d337f4452fe42d506a80103808afb0db2fd4abff6af4149f51b msg.txt
```
The output tag shoulb be:
```bash
a8061dc1305136c6c22b8baf0c0127a9
```


To verify a tag:
```bash
./poly1305-check key filename tag
```

For example
```bash
./poly1305-check 85d6be7857556d337f4452fe42d506a80103808afb0db2fd4abff6af4149f51b msg.txt a8061dc1305136c6c22b8baf0c0127a9
```
The output shoulb be:
```bash
ACCEPT
```
You can replace "msg.txt" with your prefered file name.

To clean the folder run
```bash
make clean
```

### Author
Octave Charrin
