using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net.Http;
using System.Text;
using System.Threading.Tasks;

namespace OpenIot.BoardSetup.ResourceDownload
{
    internal class ResourceDownloader
    {
        public static string GetDirectoryFromUrl(string url)
        {
            Uri uri = new Uri(url);

            string directory = uri.Host + uri.LocalPath;

            if (directory.StartsWith("www."))
                directory = directory.Substring(4);

            directory = directory.Replace("/", "\\");

            return directory;
        }

        public static void CreateFileDirectory(string filePath)
        {
            string fileDir = new FileInfo(filePath).Directory.FullName;
            CreateDirectory(fileDir);
        }

        public static void CreateDirectory(string directoryPath)
        {
            
            DirectoryInfo dirInfo = new DirectoryInfo(directoryPath);
            if (!dirInfo.Exists)
                dirInfo.Create();
        }

        public static async Task DownloadFileAsync(string sourceUrl, string destinationFile)
        {
            using HttpClient httpClient = new HttpClient();
            using HttpResponseMessage response = await httpClient.GetAsync(sourceUrl, HttpCompletionOption.ResponseHeadersRead);
            using Stream contentStream = await response.Content.ReadAsStreamAsync();

            using FileStream fileStream = new FileStream(destinationFile, FileMode.OpenOrCreate, FileAccess.Write, FileShare.None, 8192, true);

            await contentStream.CopyToAsync(fileStream);
        }

        public static async Task<Dictionary<string, string>> DownloadFilesAsync(IEnumerable<string> urls, string baseDestinationPath, bool forceOverwrite = false)
        {
            Dictionary<string, string> result = new Dictionary<string, string>();

            foreach (string url in urls)
            {
                string localDir = GetDirectoryFromUrl(url);
                string fileName = Path.Combine(baseDestinationPath, localDir);

                result.Add(url, fileName);

                if (!forceOverwrite && new FileInfo(fileName).Exists)
                    continue;

                CreateFileDirectory(fileName);

                await DownloadFileAsync(url, fileName);
            }

            return result;
        }

    }
}
