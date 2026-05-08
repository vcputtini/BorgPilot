Name:           qBorgPilot
Version:        1.1.0
Release:        1%{?dist}
Summary:        Advanced graphical interface for BorgBacku

License:        GPL-3.0-or-later
URL:            https://github.com/vcputtini/qBorgPilot
Source0:        %{name}-%{version}.tar.gz
Packager:       Volnei Cervi Puttini <vcputtini@vcplinux.com.br>

# Dependências de Build para Fedora 43 / Qt6
BuildRequires:  cmake >= 3.5
BuildRequires:  gcc-c++ >= 15.2
BuildRequires:  make >= 4.4
BuildRequires:  qt6-qtbase-devel
BuildRequires:  qt6-linguist

Requires:       borgbackup >= 1.4.4
Requires:       qt6-qtbase
Requires:       qt6-qtcharts
Requires:       polkit >= 126

%description
qBorgPilot is a management and automation tool for BorgBackup,
written in Qt6/C++.

%prep
%autosetup -n %{name}-%{version}

%build
# Fedora's native macro for configuring CMake in out-of-source mode.
%cmake \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_CXX_STANDARD=20

%cmake_build

%install
# Macro nativa do Fedora para instalação (direciona para o $RPM_BUILD_ROOT corretamente)
%cmake_install

%files
# Executável principal
%{_bindir}/%{name}

# Integração com o sistema desktop
%{_datadir}/applications/%{name}.desktop
%{_datadir}/icons/hicolor/scalable/apps/%{name}.svg

# Política do Polkit
%dir %{_datadir}/polkit-1/actions
%{_datadir}/polkit-1/actions/br.com.vcplinux.qborgpilot.policy

# Diretórios e arquivos de tradução
%dir %{_datadir}/%{name}
%dir %{_datadir}/%{name}/translations
%{_datadir}/%{name}/translations/*.qm

%changelog
* Thu May 08 2026 Volnei Cervi Puttini <vcputtini@gmail.com> - 1.1.0-1
- New release version: 1.1.0

* Thu May 07 2026 Volnei Cervi Puttini <vcputtini@gmail.com> - 1.0.1-1
- New release version: 1.0.1

* Mon May 04 2026 Volnei Cervi Puttini <vcputtini@gmail.com> - 1.0.0-1
- Fixed CMake compilation error via add_custom_target for translations
- Refactored RPM macros to comply with Fedora 43

* Sun May 03 2026 Volnei Cervi Puttini <vcputtini@gmail.com - 1.0.0-1
- Initial version of the qBorgPilot package.
