# END OF LINE MUST BE UNIX STYLE

# SET PROCEDURE: XXX_SET_COMMAND -> XXX_MANUAL_MODIFICATION

# UNSET PROCEDURE: XXX_MANUAL_MODIFICATION -> XXX_UNSET_COMMAND

# DISTRIBUTION OF VARIABLE VALUES: SYSTEM_VALUES ; USER_VALUES

# related system command
  CMD
    SETX a b
      // a=b
    REG delete HKCU\Environment /F /V a
      // delete a
  PowerShell
    [Environment]::SetEnvironmentVariable("X", "\", "User");
      // set X=\
    if ("$([Environment]::GetEnvironmentVariable("X", "User"))" -eq "") {
      [Environment]::SetEnvironmentVariable("X", "YYYYYY", "User");
    } else {
      [Environment]::SetEnvironmentVariable("X",
      "$([Environment]::GetEnvironmentVariable("X", "User"));XXXXXXXXXX;",
      "User");
    }
      // set or cat X
    [Environment]::SetEnvironmentVariable("X", "", "User");
      // delete X
