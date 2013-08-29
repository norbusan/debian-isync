Summary: Utility to synchronize IMAP mailboxes with local maildir folders
Name: isync
Version: 0.9.2
Release: 1
Copyright: GPL
Group: Applications/Internet
Source: isync-0.9.2.tar.gz
URL: http://isync.sf.net/
Packager: Oswald Buddenhagen <ossi@users.sf.net>
BuildRoot: /var/tmp/%{name}-buildroot

%description
isync is a command line utility for synchronizing a remote IMAP mailbox with a
local maildir-style mailbox.  This is useful for working in disconnected mode,
such as on a laptop.  Modifications made locally and remotely are synchronized
so that no message status flags are lost.

%prep
%setup
%build
./configure --prefix=/usr
make RPM_OPT_FLAGS="$RPM_OPT_FLAGS"

%install
make DESTDIR=$RPM_BUILD_ROOT install

%clean
rm -rf $RPM_BUILD_ROOT

%files
%doc AUTHORS COPYING README TODO ChangeLog isyncrc.sample
/usr/bin/isync
/usr/man/man1/isync.1.gz
