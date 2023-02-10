# COPYRIGHT 2023, MereGosling
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

import sys

# Have we pre-processed the valid options, or should we parse them on the fly?
preprocessed = False

class Argument:
    Name = ""
    Aliases = list()
    Value = ""
    Present = False

    def __init__(self, Name, Aliases, Value = ""):
        self.Name = Name
        self.Aliases = Aliases
        self.Value = Value

    def __str__(self):
        return f"{self.Name}:\n\tValue:\t\t'{self.Value}'\n\tPresent:\t{self.Present}\n\tAliases:\t{self.Aliases}"

ArgumentList = list()

# Process the argument list and populate our list of options.
def Preprocess(ArgMap):
    for i, arg in enumerate(ArgMap):
        ArgumentList.append(arg)

    SuppliedArg = list()

    for i, arg in enumerate(sys.argv):
        Val = ""
        if arg[0] == '/' or arg[0] == "-":
            if not (len(sys.argv) <= i+1):
                Val = sys.argv[i+1]
            SuppliedArg.append((arg[1:], Val))
        else:
            continue

    for i, arg in enumerate(ArgumentList):
        for i, x in enumerate(SuppliedArg):
            if x[0] == arg.Name or x[0] in arg.Aliases:
                arg.Present = True
                arg.Value = x[1]
                break
    
    global preprocessed
    preprocessed = True

# List all our options.
# Only works if the argument list has been preprocessed.
def ListArguments():
    if not preprocessed:
        return

    print ("--- OPTIONS ---")
    print ()
    for i, arg in enumerate(ArgumentList):
        print(arg)
        print()

# Get the value of a given option. Returns <STRING>
def GetValue(arg):
    # If we have preprocessed the argument list, look in our preprocessed list.
    if preprocessed:
        for i, o in enumerate(ArgumentList):
            if o.Name == arg:
                return o.Value
        return ""

    # Otherwise look on the fly.
    for i, arg in enumerate(sys.argv):
        if arg[1:].lower() == arg.lower():      # arg[1:] to ignore any '-' or '/' character.
            if len(sys.argv) <= i+1:
                return ""
            return sys.argv[i+1]

    return ""

# Get the value of a given switch. Returns <BOOL>
def GetSwitch(arg):
    # If we have preprocessed the argument list, look in our preprocessed list.
    if preprocessed:
        for i, o in enumerate(ArgumentList):
            if o.Name == arg:
                return o.Present
        return False

    # Otherwise look on the fly.
    for i, arg in enumerate(sys.argv):
        if arg[1:].lower() == arg.lower():        # arg[1:] to ignore any '-' or '/' character.
            return True

    return False