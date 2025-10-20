DISCRIPTION = "Network ip configuration loader"
``` bash
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"
SUMMARY = "BMC network config watcher"

SRC_URI = "file://network-watcher.cpp \
           file://network-watcher.service"

CXXFLAGS += "-std=c++23"
inherit systemd

S = "${WORKDIR}"
DEPENDS += "pkgconfig-native sdbusplus boost systemd"

SYSTEMD_AUTO_ENABLE = "enable"
SYSTEMD_SERVICE_${PN} = "network-watcher.service"

do_compile() {
    ${CXX} network-watcher.cpp -o network-watcher `pkg-config --cflags --libs sdbusplus`
}

do_install() {
    install -d ${D}${bindir}
    install -m 0755 network-watcher ${D}${bindir}
    install -d ${D}/${sysconfdir}/systemd/system
    install -m 0644 ${S}/network-watcher.service ${D}/${sysconfdir}/systemd/system
}

```


