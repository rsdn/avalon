Name:          avalon
Version:       1.0.437
Release:       1
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

%define qmake qmake
%if 0%{?fedora}
%define qmake qmake-qt4
%endif

Source0:       https://build.opensuse.org/source/home:antonbatenev:avalon/avalon/avalon_%{version}.tar.bz2
BuildRoot:     %{_tmppath}/%{name}-%{version}-build


%description
Offile client for Russian Software Developer Network


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
install -d %{buildroot}%{_datadir}/pixmaps
install -d %{buildroot}%{_datadir}/applications

install -m755 avalon %{buildroot}%{_bindir}/avalon
install -m644 src/icons/avalon.xpm %{buildroot}%{_datadir}/pixmaps/avalon.xpm
install -m644 avalon.desktop %{buildroot}%{_datadir}/applications/avalon.desktop


%clean
rm -rf %{buildroot}


%files
%defattr(-,root,root,-)
%doc README.md src/sql/avalon.mysql.sql src/sql/avalon.sqlite.sql
%{_bindir}/avalon
%{_datadir}/pixmaps/avalon.xpm
%{_datadir}/applications/avalon.desktop


%changelog
* Fri Sep 19 2014 Anton Batenev <antonbatenev@yandex.ru> 1.0.437-1
- Initial RPM release
