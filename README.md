<div align="center">

<img src="./resources/logo.png" height="150px">

# Swallow

*A minimalist functional programming language*

![](https://img.shields.io/badge/C++20-123456)
[![build](https://github.com/X-FRI/swallow/actions/workflows/build.yaml/badge.svg)](https://github.com/X-FRI/swallow/actions/workflows/build.yaml)

</div>

- Copyright (c) 2023 Muqiu Han
- This project is built using xmake: https://xmake.io/
- FAQ: https://github.com/muqiuhan/Swallow/blob/main/FAQ
- Issues: https://github.com/muqiuhan/Swallow/issues
- Contribution: https://github.com/muqiuhan/Swallow/pulls

---

![](./resources/IllegalPattern.png)

## Build & Install

To build and run with [xmake](xmake.io):
> Setup: Install the [xmake](xmake.io), [flex]() and [bison]()
  
- To build:   xmake build
- To run:     xmake run
- To install: xmake install

## Introduction
- Typechecking
  ```
  data Bool = [ True, False ]

  let main(argv) = {
    match argv with {
    | True => { 0 }
    | False => { True }
    }
  }
  ```
  ![./resources/BranchTypeChecking.png](./resources/BranchTypeChecking.png)

- Pattern match
  ```
  data List = [ Nil, Cons Int List ]

  let main(argv) = {
    match argv with {
    | Nil => { 0 }
    | Cons x xs => { 0 }
    }
  }
  ```

- [WIP] Garbage Collection
- [WIP] Polymorphism

## [LICENSE](./LICENSE)
```
Copyright (c) 2023 Muqiu Han

All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation
      and/or other materials provided with the distribution.
    * Neither the name of Terifo nor the names of its contributors
      may be used to endorse or promote products derived from this software
      without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
```