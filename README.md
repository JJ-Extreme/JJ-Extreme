# 🛡️ JJ-Extreme | Security Infrastructure Platform Engineer

Hi there! I'm JJ-Extreme, a developer specializing in the **design and implementation of security infrastructure platforms**, spanning from low-level system layers to network security, cryptographic modules, and remote isolation platforms.

Beyond simple application development, I strive to build high-trust security systems that integrate kernel/driver-level control and comply with national cryptographic standards (such as FIPS and KCMVP).

---

## 🚀 Core Expertise & Projects

### 🛰️ Drone Communication Security & Gateway
**Building a Secure Communication Platform for MAVLink-based Drone-GCS Systems**
- **Security Gateway Design**: Implemented a MAVLink traffic relay and monitoring system between GCS $\leftrightarrow$ FC using Raspberry Pi.
- **Network Optimization**: Configured secure networks through eth0/eth1 isolation, IP forwarding, and optimized routing settings.
- **Traffic Control**: Researched packet hooking and encryption/decryption structures at the TCP/UDP layer based on MAVProxy protocol relays.

### 🔐 Cryptography & Certification (FIPS/PQC)
**Designing High-Trust Crypto Modules & Evaluating Global Standards**
- **OpenSSL FIPS Provider**: Configured OpenSSL 3.x based FIPS Provider, established static linking and cross-compilation environments (`fipsmodule.cnf` and configuration optimization).
- **Post-Quantum Cryptography (PQC)**: Analyzed PQC algorithms using `liboqs` and `CLOQS`, focusing on portability for MIPS/ARM architectures.
- **Certification Standards**: Analyzed FIPS 140-3, KCMVP, and other international security certification requirements to design compliant crypto modules.
- **Licensing Systems**: Implemented a license verification system using RSA public/private key signatures with countermeasures against time rollback attacks.

### 🔑 Software HSM & Virtual Security Token
**PKCS#11 Based Software Security Token Platform**
- **Virtual USB Device**: Designed a security token architecture based on virtual USB device creation and encrypted storage (`token.dat`).
- **Interface Standardization**: Implemented universal crypto APIs providing PIN-based decryption and PKCS#11 interfaces.
- **System Integration**: Researched software-based HSM architectures integrated with the Windows driver layer.

### 🌐 Remote Browser Isolation (RBI) Platform
**Remote Browser Isolation System for Network Bridging & Internal Security**
- **Full-Stack RBI Implementation**: Designed a structure to execute browsers on the server and stream the interface to the client using Electron, React, and Puppeteer.
- **High-Precision Input Replication**: Optimized precise delivery of Keyboard/Mouse events and handled complex IME (Korean) input processing.
- **Real-time Streaming**: Implemented low-latency video streaming via Janus WebRTC with Canvas-based output rendering.

### 🛠️ System & Platform Engineering
**Multi-Platform Build Systems & Low-Level System Control**
- **Cross-Compilation**: Designed and optimized CMake toolchains for various architectures, including `aarch64` and `MIPS`.
- **Windows System Programming**: Implemented screen tampering prevention via DXGI/D3D hooking, audio device-level control, and direct NIC (Network Interface) management.
- **Platform Portability**: Established a hybrid development environment supporting Windows, Linux, macOS, and embedded systems.

---

## 🛠 Tech Stack

- **Languages**: `C/C++`, `Python`, `JavaScript/TypeScript`, `PowerShell`, `Bash`
- **Cryptography**: `OpenSSL (FIPS)`, `mbedTLS`, `liboqs`, `RSA`, `PKCS#11`
- **Frameworks & Tools**: `Electron`, `React`, `Puppeteer`, `CMake`, `MAVLink`, `Docker`
- **Platforms**: `Windows`, `Linux (Ubuntu/Debian)`, `Raspberry Pi (ARM)`, `MIPS`

---

## 📊 GitHub Stats
![JJ-Extreme's GitHub stats](https://github-readme-stats.vercel.app/api?username=JJ-Extreme&show_icons=true&theme=radical)
![Top Langs](https://github-readme-stats.vercel.app/api/top-langs/?username=JJ-Extreme&layout=compact&theme=radical)

---
*I am deeply interested in building the fundamental trust architecture of security infrastructure. I always welcome collaboration and technical discussions!*