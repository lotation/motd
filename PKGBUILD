# Maintainer: lotation <xlapsiu@gmail.com>

pkgname=cmotd
pkgver=0.1
pkgrel=1
pkgdesc="Simple MOTD tool written in C"
arch=('x86_64')
url="https://gitlab.com/lotation/motd/"
license=('GPL3')
makedepends=('git')
depends=('lsb-release' 'curl')
provides=($_pkgname)
conflicts=($_pkgname)
options=('!strip' '!emptydirs')
source=("${pkgname}::git+https://gitlab.com/lotation/motd#branch=main")
sha256sums=('SKIP')

build() {
    cd ${srcdir}/${_pkgname}
}

package() {
    cd $srcdir/$pkgname

    install -D -m 755 cmotd $pkgdir/usr/local/bin/$pkgname
}
