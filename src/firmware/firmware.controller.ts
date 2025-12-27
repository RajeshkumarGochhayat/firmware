import { Body, Controller, Header, Post } from '@nestjs/common';
import { FirmwareService } from './firmware.service';
import { GenerateFirmwareDto } from './dto/generate-firmware.dto';

@Controller('firmware')
export class FirmwareController {
  constructor(private readonly firmwareService: FirmwareService) { }

  @Post('generate')
  @Header('Content-Type', 'text/plain')
  @Header('Content-Disposition', 'attachment; filename=firmware.ino')
  generate(@Body() dto: GenerateFirmwareDto): string {
    return this.firmwareService.generate(dto.sensors);
  }
}
