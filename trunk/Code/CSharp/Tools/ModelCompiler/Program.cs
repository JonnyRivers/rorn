using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Rorn.Tools.ModelCompiler
{
    class Program
    {
        static void Main(string[] args)
        {
            if (args.Length != 2)
                DisplayHelpOnCommandLine("Insufficient argumnents");

            string sourcePathName = args[0];
            string destinationPathName = args[1];

            try
            {
                ModelCompiler modelCompiler = new ModelCompiler();
                modelCompiler.Compile(sourcePathName, destinationPathName);
            }
            catch (Exception ex)
            {
                string errorMessage = string.Format("Unexpected exception: {0}", ex.ToString());
                DisplayHelpOnCommandLine(errorMessage);
            }
        }

        static void DisplayHelpOnCommandLine(string errorMessage)
        {
            Console.WriteLine(errorMessage);
            Console.WriteLine();
            Console.WriteLine("modelcompiler source destination");
            Console.WriteLine();
            Console.WriteLine("  source      Specifies the filename of the scene to compile");
            Console.WriteLine("  destination Specifies the filename of the compiled model to create");
            Console.WriteLine();
        }
    }
}
