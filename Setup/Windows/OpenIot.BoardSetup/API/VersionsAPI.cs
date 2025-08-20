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
        private const string RequestUrl = VersionEndpointUrl + "/" + FirmwareProductName;

        static public async Task<VersionInfoResponse> ReadCurrentVersionAsync()
        {
            using HttpClient client = new HttpClient();

            HttpResponseMessage response = await client.GetAsync(RequestUrl);
            response.EnsureSuccessStatusCode();

            string responseString = await response.Content.ReadAsStringAsync();

            VersionInfoResponse responseObject = JsonSerializer.Deserialize<VersionInfoResponse>(responseString, new JsonSerializerOptions { PropertyNamingPolicy = JsonNamingPolicy.CamelCase });

            var obj = FirmwareSetupInfo.FromJson(responseObject.Version.Notes);

            return responseObject;
        }
    }
}
