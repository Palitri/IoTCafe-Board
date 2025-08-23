using OpenIot.BoardSetup.SetupConfigModels;
using OpenIot.BoardSetup.API.Models;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net.Http;
using System.Text;
using System.Text.Json;
using System.Threading.Tasks;

namespace OpenIot.BoardSetup.API
{
    internal class VersionsAPI
    {
        private const string VersionEndpointUrl = "http://api.iot.cafe/version";
        private const string FirmwareProductName = "board.firmware";
        private const string AppProductName = "app.board-setup.windows";
        private const string FirmwareRequestUrl = VersionEndpointUrl + "/" + FirmwareProductName;
        private const string AppRequestUrl = VersionEndpointUrl + "/" + AppProductName;

        private static JsonSerializerOptions serializerOptions = new JsonSerializerOptions { PropertyNamingPolicy = JsonNamingPolicy.CamelCase };

        static public async Task<VersionInfoResponse> GetAppVersionAsync()
        {
            return await RequestVersionInfoAsync(AppRequestUrl);
        }

        static public async Task<VersionInfoResponse> GetCurrentFirmwareVersionAsync()
        {
            return await RequestVersionInfoAsync(FirmwareRequestUrl);
        }

        static private async Task<VersionInfoResponse> RequestVersionInfoAsync(string url)
        {
            using HttpClient client = new HttpClient();

            HttpResponseMessage response = await client.GetAsync(url);
            response.EnsureSuccessStatusCode();

            string responseString = await response.Content.ReadAsStringAsync();

            VersionInfoResponse responseObject = JsonSerializer.Deserialize<VersionInfoResponse>(responseString, serializerOptions);

            return responseObject;
        }
    }
}
