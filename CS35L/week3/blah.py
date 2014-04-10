def ab():
    days = input("Please provide a number of days (no fractions):")
	days2 = days * 24 * 60
    print days2
    hours = input("Please provide a number of hours (no fractions):")
	hours2 = hours * 60
    print hours2
    minutes = input("Please provide a number of minutes (on fractions):")
    print minutes
    print "The equivelent in minutes is:", days2 + hours2 + minutes