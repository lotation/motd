# Maintainer: lotation <xlapsiu@gmail.com>
pkgname=motd
pkgver=0.1
pkgrel=1
pkgdesc="Simple MOTD tool written in C"
arch=(x86_64)
url="https://gitlab.com/lotation/motd"
license=('AGPLv3')
depends=('curl' 'pacutils')
makedepends=('git')
provides=(motd)
conflicts=(motd)
source=("${pkgname}::git+https://gitlab.com/lotation/motd.git")
sha256sums=('SKIP')

build() {
	cd "$pkgname"

	make
}

package() {
	cd "$pkgname"

	make install
}