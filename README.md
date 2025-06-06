# cg(1) -- manage and execute grouped commands

## NAME

cg - group multiple commands into a single reusable command

## SYNOPSIS

**cg** [**-ag** | **-lg** | **-rg** | **-dg** | **-ac** | **-lc** | **-rc** | **-dc**] [*arguments*]  
**cg** *group-name* *command-name*

## DESCRIPTION

**cg** (Command Group) is a lightweight utility for grouping multiple commands into a single, reusable command to streamline repetitive tasks. It serves as an alternative to shell aliases, allowing users to manage and execute command groups efficiently.

## OPTIONS

### Group Management

- **-ag**, **add group** *group-name*  
  Create a new command group.

- **-lg**, **list groups**  
  List all command groups.

- **-rg**, **rename group** *group-name* *command-name* *new-command-name*  
  Rename a command within a group.

- **-dg**, **remove group** *group-name*  
  Delete a command group.

### Command Management

- **-ac**, **add command** *group-name* *command-name* "*command-to-execute*"  
  Add a command to a group.

- **-lc**, **list commands** *group-name*  
  List all commands in a group.

- **-rc**, **rename command** *group-name* *command-name* *new-command-name*  
  Rename a command within a group.

- **-dc**, **remove command** *group-name* *command-name* "*command-to-execute*"  
  Remove a command from a group.

### Command Execution

- *group-name* *command-name*  
  Execute a command from a specified group.

## EXAMPLES

Create a group and add a command:

```bash
cg -ag brew
cg -ac brew update "brew update && brew upgrade"
```

Execute a command:

```bash
cg brew update
```

List all groups:

```bash
cg -lg
```

## NOTES

- Command names must be unique within a group.
- Use descriptive names for groups and commands for clarity.

## SEE ALSO

bash(1), alias(1)
