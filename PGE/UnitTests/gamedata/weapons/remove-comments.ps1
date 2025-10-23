# Cleans all comment lines in given file.
#
# How to use:
# 1.) open powershell (shift + rightclick will show that option in context menu)!
#
# 2.) enable script executing policy by entering this command:
#     Set-ExecutionPolicy RemoteSigned -Scope CurrentUser
#
# 3.) load the function from this script (notice the extra dot):
#     . .\build_funtions.ps1
#
# 4.A) Remove-CommentLines "file.txt"
#      This will create output as "file.clean.txt".
#
# 4.B) Remove-CommentLines "file.txt" -InPlace
#      This will overwrite the input file.

function Remove-CommentLines {
    [CmdletBinding()]
    param (
        [Parameter(Mandatory, Position = 0, ValueFromPipeline, ValueFromPipelineByPropertyName)]
        [string]$Path,

        [string]$Pattern = '^\s*#',  # pattern for comment lines (default: lines starting with #)
        [switch]$InPlace              # if set, modifies the file directly
    )

    begin {
        $temp = [System.IO.Path]::GetTempFileName()
    }

    process {
        if (-not (Test-Path $Path)) {
            Write-Error "File not found: $Path"
            return
        }

        Get-Content $Path | Where-Object { $_ -notmatch $Pattern } | Set-Content $temp

        if ($InPlace) {
            Move-Item -Force $temp $Path
        }
        else {
            $outPath = [System.IO.Path]::ChangeExtension($Path, '.clean.txt')
            Move-Item -Force $temp $outPath
            Write-Host "Cleaned file saved as: $outPath"
        }
    }
}
