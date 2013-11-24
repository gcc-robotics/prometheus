#!/usr/bin/env bash

VAGRANT_HOME="/home/vagrant"
BOOTSTRAP_ROOT="$VAGRANT_HOME/.vagrantboostrap"

# Password to use for MySQL root user and phpMyAdmin
DB_PASSWORD="root"

# Fix "stdin: is not a tty"
# 
# From: https://github.com/myplanetdigital/vagrant-ariadne/commit/dd0592d05d4f5c881640540fdc43d8396e00ddd7
#
# If last line is `mesg n`, replace with conditional.
if [ "`tail -1 /root/.profile`" = "mesg n" ]
then
  echo 'Fixing future `stdin: is not a tty` errors...'
  sed -i '$d' /root/.profile
  cat << 'EOH' >> /root/.profile
  if `tty -s`; then
    mesg n
  fi
EOH
fi

# Make sure bootstrap root exists
if [ ! -d $BOOTSTRAP_ROOT ];
then
	mkdir $BOOTSTRAP_ROOT
fi

# LAMP Install
if [ ! -f "$BOOTSTRAP_ROOT/LAMP" ];
then
	touch "$BOOTSTRAP_ROOT/LAMP"

	# Update first
	sudo apt-get update 2>/dev/null

	# MySQL set root pwd
	echo mysql-server-5.5  mysql-server/root_password password $DB_PASSWORD | sudo debconf-set-selections
	echo mysql-server-5.5  mysql-server/root_password_again password $DB_PASSWORD | sudo debconf-set-selections

	# Install MySQL server
	sudo apt-get -y install mysql-server 2>/dev/null

	# Seed the database if the vagrant/seed.sql file exists
	if [ -f "/vagrant/vagrant/seed.sql" ];
	then
		mysql --user=$DB_PASSWORD --password=$DB_PASSWORD < /vagrant/vagrant/seed.sql
	fi
	
	# Install apache/php etc.
	apt-get install vim apache2 php5 libapache2-mod-php5 php5-mysql php5-gd php5-curl php5-mcrypt curl unzip imagemagick git -y 2>/dev/null

	# Copy over the php.ini file from vagrant/php.ini
	if [ -f "/vagrant/vagrant/php.ini" ];
	then
		mv /etc/php5/apache2/php.ini /etc/php5/apache2/php-default.ini
		cp /vagrant/vagrant/php.ini /etc/php5/apache2/php.ini
	fi

	# Set passwords for phpMyAdmin
	echo phpmyadmin phpmyadmin/dbconfig-install boolean true | sudo debconf-set-selections
	echo phpmyadmin phpmyadmin/app-password-confirm password $DB_PASSWORD | sudo debconf-set-selections
	echo phpmyadmin phpmyadmin/mysql/admin-pass password $DB_PASSWORD | sudo debconf-set-selections
	echo phpmyadmin phpmyadmin/mysql/app-pass password $DB_PASSWORD | sudo debconf-set-selections
	echo phpmyadmin phpmyadmin/reconfigure-webserver multiselect apache2 | sudo debconf-set-selections

	# Install phpMyAdmin
	sudo apt-get -y install phpmyadmin 2>/dev/null

	# Install PHPUnit
	wget https://phar.phpunit.de/phpunit.phar
	chmod +x phpunit.phar
	mv phpunit.phar /usr/local/bin/phpunit

	# Install Composer
	curl -sS https://getcomposer.org/installer | php -- --install-dir=/usr/local/bin/
	mv /usr/local/bin/composer.phar /usr/local/bin/composer
	
	# Fix ServerName Errors
	echo ServerName $HOSTNAME > /etc/apache2/conf.d/fqdn

	# Enable modules
	sudo a2enmod rewrite
	sudo a2enmod headers
	sudo a2enmod expires

	# Change port in ports.conf and remove NameVirtualHost line
	sed -e '/NameVirtualHost/d' -e 's/80/8080/' /etc/apache2/ports.conf 1> $VAGRANT_HOME/ports.conf
	mv $VAGRANT_HOME/ports.conf /etc/apache2/ports.conf

	# Copy the apache site file
	cp /vagrant/vagrant/default /etc/apache2/sites-available/default

	# Symlink www to the vagrant directory
	ln -s /vagrant/ www

	# Restart apache
	service apache2 restart
fi