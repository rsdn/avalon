Name:          avalon
Version:       1.0.436
Release:       2
Summary:       RSDN offline client
Group:         Applications/Internet
License:       BSD-2-clause
URL:           https://github.com/rsdn/avalon
BuildRequires: libqt4-devel >= 4.4, aspell-devel, zlib-devel

%if 0%{?suse_version}
BuildRequires: libQtWebKit-devel
%endif

%if 0%{?fedora}
BuildRequires: qtwebkit-devel
%endif

Source0:       https://build.opensuse.org/source/home:antonbatenev:avalon/avalon/avalon_%{version}.tar.bz2
BuildRoot:     %{_tmppath}/%{name}-%{version}-build


%description
Offile client for Russian Software Developer Network


%define qmake qmake
%if 0%{?fedora}
%define qmake qmake-qt4
%endif


%prep
%setup -q -n avalon


%build
%{qmake} -project -recursive -Wall -nopwd -o avalon.pro \
    "CONFIG += debug_and_release" \
    "QT += network sql webkit" \
    "LIBS += -laspell -lz" \
    "DEFINES += AVALON_PACKAGE" \
    src
%{qmake} avalon.pro
make


%install
install -d %{buildroot}%{_bindir}
install -m755 avalon %{buildroot}%{_bindir}/avalon


%clean
rm -rf %{buildroot}


%files
%defattr(-,root,root,-)
%doc README.md src/sql/avalon.mysql.sql src/sql/avalon.sqlite.sql
%{_bindir}/avalon


%changelog
* Thu Sep 18 2014 Anton Batenev <antonbatenev@yandex.ru> 1.0.436-1
- Initial RPM release
