/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile$
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef cmUnixMakefileGenerator_h
#define cmUnixMakefileGenerator_h

#include "cmMakefile.h"
#include "cmMakefileGenerator.h"

/** \class cmUnixMakefileGenerator
 * \brief Write a Unix makefiles.
 *
 * cmUnixMakefileGenerator produces a Unix makefile from its
 * member m_Makefile.
 */
class cmUnixMakefileGenerator : public cmMakefileGenerator
{
public:
  ///! Set cache only and recurse to false by default.
  cmUnixMakefileGenerator();

  virtual ~cmUnixMakefileGenerator();
  
  ///! Get the name for the generator.
  virtual const char* GetName() {return "Unix Makefiles";}

  ///! virtual copy constructor
  virtual cmMakefileGenerator* CreateObject() 
    { return new cmUnixMakefileGenerator;}
  
  //! just sets the Cache Only and Recurse flags
  virtual void SetLocal(bool local);

  /**
   * If cache only is on.
   * and only stub makefiles are generated, and no depends, for speed.
   * The default is OFF.
   **/
  void SetCacheOnlyOn()  {m_CacheOnly = true;}
  void SetCacheOnlyOff()  {m_CacheOnly = false;}
  /**
   * If recurse is on, then all the makefiles below this one are parsed as well.
   */
  void SetRecurseOn() {m_Recurse = true;}
  void SetRecurseOff() {m_Recurse = false;}
  
  /**
   * Produce the makefile (in this case a Unix makefile).
   */
  virtual void GenerateMakefile();

  /**
   * Output the depend information for all the classes 
   * in the makefile.  These would have been generated
   * by the class cmMakeDepend.
   */
  virtual bool OutputObjectDepends(std::ostream&);

  /**
   * Output the check depend information for all the classes 
   * in the makefile.  These would have been generated
   * by the class cmMakeDepend.
   */
  virtual void OutputCheckDepends(std::ostream&);

  /**
   * Try to determine system infomation such as shared library
   * extension, pthreads, byte order etc.  
   */
  virtual void EnableLanguage(const char*);

protected:
  virtual void RecursiveGenerateCacheOnly();
  virtual void ProcessDepends(const cmMakeDepend &md);
  virtual void GenerateCacheOnly();
  virtual void OutputMakefile(const char* file);
  virtual void OutputTargetRules(std::ostream& fout);
  virtual void OutputLinkLibraries(std::ostream&, const char* name, const cmTarget &);

  virtual void OutputSharedLibraryRule(std::ostream&, const char* name,
                                       const cmTarget &);
  virtual void OutputModuleLibraryRule(std::ostream&, const char* name, 
                                       const cmTarget &);
  virtual void OutputStaticLibraryRule(std::ostream&, const char* name,
                                       const cmTarget &);
  virtual void OutputExecutableRule(std::ostream&, const char* name,
                                    const cmTarget &);
  virtual void OutputUtilityRule(std::ostream&, const char* name,
                                 const cmTarget &);
  
  virtual void OutputTargets(std::ostream&);
  virtual void OutputSubDirectoryRules(std::ostream&);
  virtual void OutputDependLibs(std::ostream&);
  virtual void OutputLibDepend(std::ostream&, const char*);
  virtual void OutputExeDepend(std::ostream&, const char*);
  virtual void OutputCustomRules(std::ostream&);
  virtual void OutputMakeVariables(std::ostream&);
  virtual void OutputMakeRules(std::ostream&);
  virtual void OutputInstallRules(std::ostream&);
  virtual void OutputSourceObjectBuildRules(std::ostream& fout);
  virtual void OutputBuildObjectFromSource(std::ostream& fout,
                                           const char* shortName,
                                           const cmSourceFile& source,
                                           const char* extraCompileFlags,
                                           bool sharedTarget);
  
  virtual void BuildInSubDirectory(std::ostream& fout,
                                   const char* directory,
                                   const char* target1,
                                   const char* target2,
                                   bool silent = false);

  virtual void OutputSubDirectoryVars(std::ostream& fout,
                                      const char* var,
                                      const char* target,
                                      const char* target1,
                                      const char* target2,
                                      const char* depend,
                                      const std::vector<std::string>&
                                      SubDirectories,
                                      bool silent = false);

  virtual void OutputMakeRule(std::ostream&, 
                              const char* comment,
                              const char* target,
                              const char* depends, 
                              const char* command,
                              const char* command2 = 0,
                              const char* command3 = 0,
                              const char* command4 = 0);
  virtual void OutputBuildTargetInDir(std::ostream& fout,
                                      const char* path,
                                      const char* library,
                                      const char* fullpath,
                                      const char* outputPath);
  ///! return true if the two paths are the same
  virtual bool SamePath(const char* path1, const char* path2);
  virtual std::string GetOutputExtension(const char* sourceExtension);
  virtual void OutputIncludeMakefile(std::ostream&, const char* file);
  void SetObjectFileExtension(const char* e) { m_ObjectFileExtension = e;}
  void SetExecutableExtension(const char* e) { m_ExecutableExtension = e;}
  void SetStaticLibraryExtension(const char* e) {m_StaticLibraryExtension = e;}
  void SetSharedLibraryExtension(const char* e) {m_SharedLibraryExtension = e;}
  void SetLibraryPrefix(const char* e) { m_LibraryPrefix = e;}
  // convert a path to an output path for unix this will call
  // ConvertToUnixOutputPath
  virtual std::string ConvertToOutputPath(const char* s) 
    { return cmSystemTools::ConvertToOutputPath(s);  }
  std::string CreateTargetRules(const cmTarget &target,
                                const char* targetName);
  virtual std::string CreateMakeVariable(const char* s, const char* s2)
    {
      return std::string(s) + std::string(s2);
    }
  
  ///! if the OS is case insensitive then return a lower case of the path.
  virtual std::string LowerCasePath(const char* path)
    {
      return std::string(path);
    }
  
  /**
   * Try running cmake and building a file. This is used for dynalically
   * loaded commands, not as part of the usual build process. For the
   * generator, it can assume that cmake has been run on the srcdir/bindir
   * and it just needs to be compiled.  
   */
  virtual int TryCompile(const char *srcdir, const char *bindir,
                         const char *projectName);
  
protected:
  std::string m_ExecutableOutputPath;
  std::string m_LibraryOutputPath;
  std::string m_SharedLibraryExtension;
  std::string m_ObjectFileExtension;
  std::string m_ExecutableExtension;
  std::string m_StaticLibraryExtension;
  std::string m_LibraryPrefix;
private:
  bool m_CacheOnly;
  bool m_Recurse;
};

#endif
