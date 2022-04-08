import "pe"

rule nastyware_branch1
{
    meta:
        description = "Rule based in branch 1 from DAMICORE result of Nastyware study."
        author = "Nilson Sangy"
        date = "2022-04-05"
        rule_version = "v1.0"
        malware_type = "ransomware"

    strings:
        $s1 = "Microsoft (R) Windows (R) Operating System"
    
    condition:
        (pe.imports("mscoree.dll", "_CorExeMain") and not $s1)
    
}


rule nastyware_branch2
{
    meta:
        description = "Rule based in branch 2 from DAMICORE result of Nastyware study."
        author = "Nilson Sangy"
        date = "2022-04-05"
        rule_version = "v1.0"
        malware_type = "ransomware"

    strings:
        $s1 = "Microsoft Corporation"
        $s2 = "Microsoft (R) Windows (R) Operating System"

        $g1 = "adprep.dll"
        $g2 = "dfshim.dll"
        $g3 = {64 67 67 70 65 78 74} // dggpext
        $g4 = "mscoree.dll"
        $g5 = "msdadiag.dll"
        $g6 = "netfxperf.dll"
        $g7 = "WindowsAccessBridge-64.dll"
    
    condition:
        ( pe.imports("kernel32.dll", "GetProcAddress") and pe.imports("kernel32.dll", "ExitProcess") )
        and not
        (   pe.imports("msvcrt.dll", "_amsg_exit")
            or pe.imports("msvcrt.dll", "__C_specific_handler")
            or pe.imports("WS2_32.dll", "recv")
            or pe.imports("api-ms-win-core-libraryloader-l1-2-0.dll", "DisableThreadLibraryCalls")
            or any of them )

}


rule nastyware_branch3
{
    meta:
        description = "Rule based in branch 3 from DAMICORE result of Nastyware study."
        author = "Nilson Sangy"
        date = "2022-04-05"
        rule_version = "v1.0"
        malware_type = "ransomware"

    strings:
        $s1 = "adal.dll"
        $s2 = "vm3dgl64"
        $s3 = "vm3dservice"
    
    condition:
        ( pe.imports("kernel32.dll", "ExitProcess") and pe.imports("kernel32.dll", "RtlUnwind") and not any of them )

}
