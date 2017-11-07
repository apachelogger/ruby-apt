/*
    Copyright 2017 Harald Sitter <sitter@kde.org>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of
    the License or (at your option) version 3 or any later version
    accepted by the membership of KDE e.V. (or its successor approved
    by the membership of KDE e.V.), which shall act as a proxy
    defined in Section 14 of version 3 of the license.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

  #include "rice/Data_Type.hpp"
  #include "rice/Constructor.hpp"

  #include <apt-pkg/cachefile.h>
  #include <apt-pkg/progress.h>
  #include <apt-pkg/cacheiterators.h>
  #include <apt-pkg/init.h>

  using namespace Rice;

  // class Init {
  // public:
  //   static void init()
  //   {
  //     static Init
  //   }
  //   bool initd = false;
  // }

  class InitException : public std::exception
{
  virtual const char* what() const throw()
  {
    return "My exception happened";
  }
} ;


  extern "C"
  void Init_test()
  {
    if (!pkgInitConfig(*_config) || !pkgInitSystem(*_config, _system)) {
        // RAISE ERROR
        throw InitException();
        return;
    }


    Data_Type<OpProgress> rb_cOpProgress =
      define_class<OpProgress>("OpProgress")
      .define_constructor(Constructor<OpProgress>());

    // Defined so
    define_class<pkgCache::Iterator<pkgCache::Package, pkgCache::PkgIterator> >("pkgCache::Iterator<pkgCache::Package, pkgCache::PkgIterator>")
    .define_method("end", &pkgCache::PkgIterator::end)
    ;

    Data_Type<pkgCache::PkgIterator> rb_cPkgIterator =
      define_class<pkgCache::PkgIterator, pkgCache::Iterator<pkgCache::Package, pkgCache::PkgIterator> >("PkgIterator")
      ;

    typedef pkgCache::PkgIterator (pkgCache::*pkgCacheFindPkg)(const std::string &);
    Data_Type<pkgCache> rb_cPkgCache =
      define_class<pkgCache>("PkgCache")
      // .define_class("pkgIterator")
      // FIXME: need a wrapper here if arch is nil it should not call the arch overload
      // .define_method("findPkg", pkgCacheFindPkg(&pkgCache::FindPkg), (Arg("name"), Arg("arch") = "amd64"))
      .define_method("findPkg", pkgCacheFindPkg(&pkgCache::FindPkg))
      .define_iterator(&pkgCache::PkgBegin, &pkgCache::PkgEnd)
      ;

    Data_Type<pkgCache::Package> rb_cPkgCachePackage =
      define_class_under<pkgCache::Package>(rb_cPkgCache, "Package")
      ;

    Data_Type<pkgDepCache> rb_cPkgDepCache =
      define_class<pkgDepCache>("PkgDepCache")
      .define_method("pkgBegin", &pkgCache::PkgBegin)
      ;

    // Data_Type<pkgDepCache::PkgIterator> rb_cPkgDepCachePkgIterator =
    //   define_class_under<pkgDepCache::PkgIterator>(rb_cPkgDepCache, "PkgIterator")
    //   .define_method("end", &pkgDepCache::PkgIterator::end)
    //   ;

    typedef bool (pkgCacheFile::*open)(OpProgress *, bool);
    Data_Type<pkgCacheFile> rb_cTest =
      define_class<pkgCacheFile>("PkgCacheFile")
      .define_constructor(Constructor<pkgCacheFile>())
      .define_method("isDepCacheBuilt", &pkgCacheFile::IsDepCacheBuilt)
      .define_method("close", &pkgCacheFile::Close)
      .define_method("open", open(&pkgCacheFile::Open), (Arg("progress") = NULL, Arg("with_lock") = (bool)true))
      .define_method("readOnlyOpen", &pkgCacheFile::ReadOnlyOpen, (Arg("progress") = NULL))
      .define_method("pkgCache", &pkgCacheFile::GetPkgCache)
      .define_method("pkgDepCache", &pkgCacheFile::GetDepCache)
      ;
  }
