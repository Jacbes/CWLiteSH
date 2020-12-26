pkgname=LiteSH
_pkgname=LiteSH
pkgver=1.0
pkgrel=1
pkgdesc="CWLitesh create and manage processes"
url="https://github.com/Jacbes/CWLiteSH"
arch=('x86_64')
license=('custom')
makedepends=('git' 'make')
source=("git+$url")
md5sums=('SKIP')

build() {
    cd "$srcdir/CW$pkgname"
    make
}

package() {
    cd "$srcdir/CW$pkgname"
	make DESTDIR="${pkgdir}" install
}
